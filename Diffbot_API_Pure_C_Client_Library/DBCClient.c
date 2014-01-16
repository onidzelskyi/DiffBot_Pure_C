/**
 * @file DBCClient.c
 * @author Oleksii Nidzelskyi <alexey.education@gmail.com>
 * @link https://github.com/onidzelskyi/DiffBot_Pure_C
 * @copyright Copyright &copy; 2014 Ukraine
 * @license MIT
 * @since 0.1
 */


#include "DBCClient.h"
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>


#define DELETE(ptr) if(ptr) { free(ptr); ptr=0; }


#define STRCPY(dst, src) if(dst) {DELETE(dst)} \
                        dst = strdup(src);


#define APPEND(dst, src) if(!dst) {dst=malloc(strlen(src)+1);} \
                        else {dst = realloc(dst, strlen(dst)+strlen(src)+1);} \
                        strncat(dst, src, strlen(src));


const char* URI_HEAD = "http://api.diffbot.com/v";
char* json;
 

typedef struct MemoryStruct_s {
    char *memory;
    size_t size;
}MemoryStruct;



static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct* mem = (MemoryStruct*)userp;
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}


json_object* diffBot(DBRequest* request) {
    int ret = OK;
    char* uri = 0;
    CURL* _curl;
    CURLcode result;
    MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;
    _curl = curl_easy_init();
    curl_global_init(CURL_GLOBAL_ALL);
    STRCPY(uri, URI_HEAD);
    APPEND(uri, request->version);
    APPEND(uri, "/");
    APPEND(uri, request->api);
    APPEND(uri, "?token=");
    APPEND(uri, request->token);
    int i;
    for(i=0; i<request->count && ret==OK; ++i) {
        APPEND(uri, "&");
        APPEND(uri, request->keys[i]);
        APPEND(uri, "=");
        APPEND(uri, request->values[i]);
    }
    curl_easy_setopt(_curl, CURLOPT_URL, uri);
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    DELETE(json)
    curl_easy_setopt(_curl,   CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    result = curl_easy_perform(_curl);
    if(result != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(result));
    }
    json_object* new_obj = json_object_new_string(chunk.memory);
    request->error = result;
    curl_easy_cleanup(_curl);
    curl_global_cleanup();
    DELETE(chunk.memory);
    chunk.size = 0;
    DELETE(uri)
    return new_obj;
}


int addDBRequestField(DBRequest* request, const char* key, const char* value) {
    int ret = ERROR;
    if(request->count<MAX_NUM_FIELDS) {
        STRCPY(request->keys[request->count], key);
        STRCPY(request->values[request->count], value);
        ++request->count;
    }
    return ret;
}


DBRequest* initDBRequest() {
    return (DBRequest*)calloc(1, sizeof(DBRequest));
}


void cleanDBRequest(DBRequest* request) {
    if(request) {
        int i;
        for (i=0; i<request->count; ++i) {
            DELETE(request->keys[i])
            DELETE(request->values[i])
        }
        request->count = 0;
        request->error = OK;
        DELETE(request->version)
        DELETE(request->api)
        DELETE(request->token)
        DELETE(request)
    }
}


void addDBVersion(DBRequest* request, const char* version) {
    if(request) {
        STRCPY(request->version, version);
    }
}


void addDBAPI(DBRequest* request, const char* api) {
    if(request) {
        STRCPY(request->api, api);
    }
}


void addDBToken(DBRequest* request, const char* token) {
    if(request) {
        STRCPY(request->token, token);
    }
}
