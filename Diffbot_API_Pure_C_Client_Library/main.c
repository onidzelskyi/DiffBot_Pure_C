//
//  main.c
//  Diffbot_API_Pure_C_Client_Library
//
//  Created by Oleksii on 12/31/13.
//  Copyright (c) 2013 Oleksii. All rights reserved.
//
// valgrind --track-origins=yes --leak-check=full --show-reachable=yes -v --log-file=valgrind.log ./diffBootCClient

/**
 *     URI: http://api.diffbot.com/v2/article?token=4f558dfde45e6d0e1850da93954c91b1&url=http://allthingsd.com/20120516/google-gets-semantic-launches-knowledge-graph-in-english-starting-today
 *
 */


#include <stdio.h>
#include "DBCClient.h"
#include "json.h"

int main(int argc, const char * argv[]) {
    //!<
    //!< First of all create and init control structure
    //!<
    DBRequest* request = initDBRequest();
    
    //!<
    //!< Next, add version (currently supported version 1 and 2)
    //!<
    addDBVersion(request, "2");
    
    //!<
    //!< Next, add product category
    //!< currently supported product categories:
    //!< article
    //!< frontpage
    //!< product
    //!< image
    //!< classifier
    //!<
    addDBAPI(request, "article");
    
    //!<
    //!< Next, add token
    //!<
    addDBToken(request, "4f558dfde45e6d0e1850da93954c91b1");
    
    //!<
    //!< Next, fill up fields
    //!<
    addDBRequestField(request, "url", "http://allthingsd.com/google-gets-semantic-launches-knowledge-graph-in-english-starting");
    
    //!<
    //!< At last, make request and take response back
    //!<
    json_object* json = diffBot(request);

    //!<
    //!< Print out result
    //!<
    printf("%s\n", json_object_to_json_string(json));
    /*
    addDBAPI(&request, "image");
    printf("\n\n%s\n", json_object_to_json_string(diffBot(&request)));
    
    addDBAPI(&request, "frontpage");
    printf("\n\n%s\n", json_object_to_json_string(diffBot(&request)));
    
    addDBAPI(&request, "product");
    printf("\n\n%s\n", json_object_to_json_string(diffBot(&request)));
     */
    
    //!<
    //!< Clean up resources
    //!<
    json_object_put(json);
    cleanDBRequest(request);
    return 0;
}

