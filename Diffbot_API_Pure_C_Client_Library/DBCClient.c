/**
 * @file DBCClient.c
 * @author Oleksii Nidzelskyi <alexey.education@gmail.com>
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
 

/**
 *  User defined memory structure for transfering the data
 */
typedef struct MemoryStruct_s {
    char *memory;
    size_t size;
}MemoryStruct;



/** @brief Callback function. 
 *  This function gets called by libcurl as soon as there is data received that needs to be saved
 *  @param contents Pointer to the data
 *  @param size The size of the data pointed to by ptr
 *  @param nmemb Number of data blocks
 *  @param userp Pointer to user defined data where recieved data will be copied
 *  @return The number of bytes actually taken care of.
 *  If that amount differs from the amount passed to your function, 
 *  it'll signal an error to the library. 
 *  This will abort the transfer and return CURLE_WRITE_ERROR
 */
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
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


/** @brief Perform client API request
 *  @param request Pointer to the DBRequest control structure.
 *  @return pointer to newly created json object
 */
json_object* diffBot(DBRequest* request) {
    //!<
    //!< Data block
    //!<
    char* uri = 0;
    CURL* _curl;
    CURLcode result;
    MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;
    //!<
    //!< Initialize curl
    //!<
    _curl = curl_easy_init();
    curl_global_init(CURL_GLOBAL_ALL);
    //!<
    //!< Initialize url sring
    //!<
    STRCPY(uri, URI_HEAD);
    APPEND(uri, request->version);
    APPEND(uri, "/");
    APPEND(uri, request->api);
    APPEND(uri, "?token=");
    APPEND(uri, request->token);
    int i;
    for(i=0; i<request->count; ++i) {
        APPEND(uri, "&");
        APPEND(uri, request->keys[i]);
        APPEND(uri, "=");
        APPEND(uri, request->values[i]);
    }
    //!<
    //!< Initialize curl
    //!<
    curl_easy_setopt(_curl, CURLOPT_URL, uri);
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    DELETE(json)
    curl_easy_setopt(_curl,   CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    //!<
    //!< Perform request
    //!<
    result = curl_easy_perform(_curl);
    if(result != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(result));
    }
    //!<
    //!< Create json object from response
    //!<
    json_object* new_obj = json_object_new_string(chunk.memory);
    request->error = result;
    //!<
    //!< Cleanup
    //!<
    curl_easy_cleanup(_curl);
    curl_global_cleanup();
    DELETE(chunk.memory);
    chunk.size = 0;
    DELETE(uri)
    return new_obj;
}


/** @brief Initialize key and value pair of the DBRequest control structure
 *  @param request Pointer to the DBRequest control structure.
 *  @param name Pointer to the null-terminated C-string
 *  @param value Pointer to the null-terminated C-string
 */
void addDBRequestField(DBRequest* request, const char* key, const char* value) {
    if(request->count<MAX_NUM_FIELDS) {
        STRCPY(request->keys[request->count], key);
        STRCPY(request->values[request->count], value);
        ++request->count;
    }
}


/** @brief Create and inialize control structure DBRequest
 *  @return pointer to newly created control structure DBRequest
 *  or null if there not enought memory to allocate it.
 */
DBRequest* initDBRequest() {
    return (DBRequest*)calloc(1, sizeof(DBRequest));
}


/** @brief clean memory allocated by the DBRequest control structure
 *  @param request Pointer to the DBRequest control structure.
 */
void cleanDBRequest(DBRequest* request) {
    if(request) { //!< if the memory has been allocated
        int i;
        for (i=0; i<request->count; ++i) { //!< for each field in request
            //!< free memory allocated by the addDBRequestField function
            DELETE(request->keys[i])
            DELETE(request->values[i])
        }
        //!< Reset counters and errors
        request->count = 0;
        request->error = OK;
        //!< free memory allocated by the addDBVersion function
        DELETE(request->version)
        //!< free memory allocated by the addDBAPI function
        DELETE(request->api)
        //!< free memory allocated by the addDBToken function
        DELETE(request->token)
        //!< free memory allocated by the control structure itself
        DELETE(request)
    }
}


/** @brief Initialize version API of the DBRequest control structure
 *  @param request Pointer to the DBRequest control structure.
 *  @param version Pointer to the null-terminated C-string
 */
void addDBVersion(DBRequest* request, const char* version) {
    if(request) {
        STRCPY(request->version, version);
    }
}


/** @brief Initialize API name of the DBRequest control structure
 *  @param request Pointer to the DBRequest control structure.
 *  @param api Pointer to the null-terminated C-string
 */
void addDBAPI(DBRequest* request, const char* api) {
    if(request) {
        STRCPY(request->api, api);
    }
}


/** @brief Initialize token value of the DBRequest control structure
 *  @param request Pointer to the DBRequest control structure.
 *  @param token Pointer to the null-terminated C-string
 */
void addDBToken(DBRequest* request, const char* token) {
    if(request) {
        STRCPY(request->token, token);
    }
}
