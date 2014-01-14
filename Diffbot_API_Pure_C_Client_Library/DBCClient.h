//
//  DBCClient.h
//  Diffbot_API_Pure_C_Client_Library
//
//  Created by Oleksii on 12/31/13.
//  Copyright (c) 2013 Oleksii. All rights reserved.
//

#include <string.h>
#include <stdlib.h>
#include "json.h"


#ifndef Diffbot_API_Pure_C_Client_Library_DBCClient_h
#define Diffbot_API_Pure_C_Client_Library_DBCClient_h


#define MAX_NUM_FIELDS 128


enum {OK, ERROR, OVERFULL};


typedef struct DBRequest {
    unsigned count;
    char* version;
    char* api;
    char* token;
    char* keys[MAX_NUM_FIELDS];
    char* values[MAX_NUM_FIELDS];
    int error;
}DBRequest;


DBRequest* initDBRequest();


void cleanDBRequest(DBRequest*);


void addDBVersion(DBRequest*, const char*);


void addDBAPI(DBRequest*, const char*);


void addDBToken(DBRequest*, const char*);


int addDBRequestField(DBRequest*, const char*, const char*);


json_object* diffBot(DBRequest*);




#endif
