//
//  DBCClient.c
//  Diffbot_API_Pure_C_Client_Library
//
//  Created by Oleksii on 12/31/13.
//  Copyright (c) 2013 Oleksii. All rights reserved.
//


#include "DBCClient.h"
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#define DELETE(ptr) if(ptr) { free(ptr); ptr=0; }

/*
#define STRCPY(dst, src) if(dst) {DELETE(dst)} \
                        dst = malloc(strlen(src)+1); \
                        if(dst) {strncat(dst, src, strlen(src));}
*/
#define STRCPY(dst, src) if(dst) {DELETE(dst)} \
                        dst = strdup(src);


#define APPEND(dst, src) if(!dst) {dst=malloc(strlen(src)+1);} \
                        else {dst = realloc(dst, strlen(dst)+strlen(src)+1);} \
                        strncat(dst, src, strlen(src));


const char* URI_HEAD = "http://api.diffbot.com/v";
/*
char* URI = 0;
unsigned long total_len;
const char* AND = "&";
const char* SET = "=";
 */
char* json;
 
/*
static size_t write_data(void *ptr, size_t size, size_t nmemb, void* buf) {
    json = malloc(nmemb * size);
    json = strndup(ptr, (size_t)(size *nmemb));
    printf("%s", json);
    return size * nmemb;
}
*/
typedef struct MemoryStruct {
    char *memory;
    size_t size;
}MemoryStruct;

 static size_t
 WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
 {
 size_t realsize = size * nmemb;
 MemoryStruct* mem = (MemoryStruct*)userp;
 
 mem->memory = realloc(mem->memory, mem->size + realsize + 1);
 if(mem->memory == NULL) {
 // out of memory!
printf("not enough memory (realloc returned NULL)\n");
return 0;
}

memcpy(&(mem->memory[mem->size]), contents, realsize);
mem->size += realsize;
mem->memory[mem->size] = 0;

return realsize;
}
/*
static size_t write_data(void *ptr, size_t size, size_t nmemb, void* buf) {
//    APPEND(json, ptr)
 
    if(json) {
        size_t len = strlen(json);
        json = (char*)realloc(json, len+size*nmemb+1);
        strcat(json, ptr);
    } else {
        json = malloc(nmemb * size);
        json = strndup(ptr, (size_t)(size *nmemb));
    }
 
    return size * nmemb;
}
*/
/*
int addField(const char* key, const char* value) {
    int ret = 0;
    if(key && value) {
        unsigned long key_len = strlen(key);
        unsigned long value_len = strlen(value);
        URI = (char*)realloc((void*)URI, total_len +key_len+value_len+3);
        if(URI) {
            strcat(URI, key);
            strcat(URI, SET);
            strcat(URI, value);
            strcat(URI, AND);
            total_len = strlen(URI);
            ret = 1;
        }
    }
    return ret;
}


int addHeader(DBRequest* request) {
    int ret = 0;
    DELETE(URI)
    size_t version_len = strlen(request->version);
    size_t product_len = strlen(request->product);
    size_t head_len = strlen(URI_HEAD);
    URI = (char*)malloc(head_len+version_len+product_len+3);
    if(URI) {
        strcpy(URI, URI_HEAD);
        strncat(URI, request->version, version_len);
        strncat(URI, "/", 1);
        strncat(URI, request->product, product_len);
        strncat(URI, "?", 1);
        total_len = strlen(URI);
        ret = 1;
    }
    return ret;
}
*/
json_object* diffBot(DBRequest* request) {
    int ret = OK;
    char* uri = 0;
    CURL* _curl;
    CURLcode result;
    MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;
    _curl = (curl_easy_init());
    //int ret = addHeader(request);
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
    //printf("REQUEST: %s\n\n", uri);
    //return 0;
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
    json_object *new_obj = json_object_new_string(chunk.memory);
    //json_object* new_obj = json_tokener_parse(chunk.memory);
    request->error = result;
    curl_easy_cleanup(_curl);
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
    //!<
    //!< If key already exist replace value if not appropriate
    //!<
        /*
         for (int i=0; i<request->count; ++i) {
        if (request->keys[i] && !strcmp(request->keys[i], key)) {
            ret = OK;
            if(strcmp(request->values[i], value)) {
                DELETE(request->values[i])
                request->values[i] = (char*)malloc(strlen(value)+1);
                strcpy(request->values[i], value);
            }
        }
    }
    //!<
    //!< If key not exist
    //!<
    if(ret!=OK) {
        if(request->count==MAX_NUM_FIELDS-1) { //!< buffer already full
            request->error = OVERFULL;
        } else { //!< we have enought room to place field
            request->keys[request->count] = (char*)malloc(strlen(key)+1);
            strcpy(request->keys[request->count], key);
            request->values[request->count] = (char*)malloc(strlen(value)+1);
            strcpy(request->values[request->count], value);
            request->count++;
            ret = OK;
        }
    }
         */
    return ret;
}


void initDBRequest(DBRequest* request) {
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
}


void addDBVersion(DBRequest* request, const char* version) {
    STRCPY(request->version, version);
}


void addDBAPI(DBRequest* request, const char* api) {
    STRCPY(request->api, api);
}


void addDBToken(DBRequest* request, const char* token) {
    STRCPY(request->token, token);
}

/*
char* diffbot(const char* url, const char* token, const char* api, const Field fields[], const char* version) {
    CURL* _curl;
    CURLcode result;
    _curl = (curl_easy_init());
    char* uri=0;
    char* json;
    APPEND(uri, URI_HEAD);
    APPEND(uri, version);
    APPEND(uri, "/");
    APPEND(uri, api);
    APPEND(uri, "?token=");
    APPEND(uri, token);
    printf("%d\n", sizeof(fields));
    APPEND(uri, "&");
    return uri;
    curl_easy_setopt(_curl, CURLOPT_URL, URI);
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(_curl,   CURLOPT_WRITEDATA, json);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_data);
    result = curl_easy_perform(_curl);
    if(result != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(result));
    }
    curl_easy_cleanup(_curl);
    return json;
}
*/
