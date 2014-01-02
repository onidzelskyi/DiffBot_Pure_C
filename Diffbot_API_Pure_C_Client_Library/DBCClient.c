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


const char* URI_HEAD = "http://api.diffbot.com/v2/article?";
char* URI = 0;
unsigned long total_len;
const char* AND = "&";
const char* SET = "=";
char* json;
/*
static size_t write_data(void *ptr, size_t size, size_t nmemb, void* buf) {
    json = malloc(nmemb * size);
    json = strndup(ptr, (size_t)(size *nmemb));
    printf("%s", json);
    return size * nmemb;
}
*/
static size_t write_data(void *ptr, size_t size, size_t nmemb, void* buf) {
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


int addHeader() {
    int ret = 0;
    DELETE(URI)
    URI = (char*)malloc(strlen(URI_HEAD)+1);
    if(URI) {
        strcpy(URI, URI_HEAD);
        total_len = strlen(URI);
        ret = 1;
    }
    return ret;
}

char* diffBot(DBRequest* request) {
    CURL* _curl;
    CURLcode result;
    _curl = (curl_easy_init());
    int ret = addHeader();
    int i = 0;
    while(i<request->count && ret) {
        ret = addField(request->keys[i], request->values[i]);
        ++i;
    }
    //printf("URI: %s\n", URI);
    curl_easy_setopt(_curl, CURLOPT_URL, URI);
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    DELETE(json)
    curl_easy_setopt(_curl,   CURLOPT_WRITEDATA, json);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_data);
    result = curl_easy_perform(_curl);
    if(result != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(result));
    }
    request->error = result;
    curl_easy_cleanup(_curl);
    return json;
}


int addDBRequestField(DBRequest* request, const char* key, const char* value) {
    int ret = ERROR;
    //!<
    //!< If key already exist replace value if not appropriate
    //!<
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
    return ret;
}


void initDBRequest(DBRequest* request) {
    for (int i=0; i<request->count; ++i) {
        DELETE(request->keys[i])
        DELETE(request->values[i])
    }
    request->count = 0;
    request->error = OK;
}
