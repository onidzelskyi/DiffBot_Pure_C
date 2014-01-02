//
//  main.c
//  Diffbot_API_Pure_C_Client_Library
//
//  Created by Oleksii on 12/31/13.
//  Copyright (c) 2013 Oleksii. All rights reserved.
//

/**
 *     URI: http://api.diffbot.com/v2/article?token=4f558dfde45e6d0e1850da93954c91b1&url=http://allthingsd.com/20120516/google-gets-semantic-launches-knowledge-graph-in-english-starting-today
 *
 */
#include <stdio.h>
#include "DBCClient.h"


int main(int argc, const char * argv[])
{
    //!<
    //!< First of all define control structure
    //!<
    DBRequest request;
    //!<
    //!< Then, initialize it
    //!<
    initDBRequest(&request);
    //!<
    //!< Next, fill up fields
    //!<
    addDBRequestField(&request, "token", "4f558dfde45e6d0e1850da93954c91b1");
//    addDBRequestField(&request, "url", "http://allthingsd.com/20120516/google-gets-semantic-launches-knowledge-graph-in-english-starting-today");
    addDBRequestField(&request, "url", "http://allthingsd.com/google-gets-semantic-launches-knowledge-graph-in-english-starting");
    //!<
    //!< At last, make request and take response back
    //!<
    char* json = diffBot(&request);
    //!<
    //!< Print out result
    //!<
    printf("\n\n%s\n", json);

    
/*
    addDBRequestField(&request, "token", "4f558dfde45e6d0e1850da93954c91b1");
    addDBRequestField(&request, "url", "http://allthingsd.com/google-gets-semantic-launches-knowledge-graph-in-english-starting");
    json = diffBot(&request);
    printf("\n\n%s\n", json);
 )
 */
    return 0;
}

