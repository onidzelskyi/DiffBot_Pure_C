//
//  DBCClient.h
//  Diffbot_API_Pure_C_Client_Library
//
//  Created by Oleksii on 12/31/13.
//  Copyright (c) 2013 Oleksii. All rights reserved.
//

#include <string.h>
#include <stdlib.h>


#ifndef Diffbot_API_Pure_C_Client_Library_DBCClient_h
#define Diffbot_API_Pure_C_Client_Library_DBCClient_h


#define MAX_NUM_FIELDS 128

enum {OK, ERROR, OVERFULL};


typedef struct DBRequest {
    unsigned count;
    char* keys[MAX_NUM_FIELDS];
    char* values[MAX_NUM_FIELDS];
    int error;
}DBRequest;

void initDBRequest(DBRequest*);

int addDBRequestField(DBRequest*, const char*, const char*);

struct JsonObj;

extern char* diffBot(DBRequest*);

#endif
