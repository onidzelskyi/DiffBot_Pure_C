/**
 * @file main.c
 * @author Oleksii Nidzelskyi <alexey.education@gmail.com>
 * @copyright Copyright &copy; 2014 Ukraine
 * @license MIT
 * @since 0.1
 *
 * This is sample application to send http GET request like this one:
 *
 * URI: http://api.diffbot.com/v2/article?token=4f558dfde45e6d0e1850da93954c91b1&url=http://allthingsd.com/20120516/google-gets-semantic-launches-knowledge-graph-in-english-starting-today
 *
 * NOTE: Token value isn't valid - it's only demo
 *
 */


//!<
//!< Run the this command for memory leaks (Linux only!)
//!<
//!< valgrind --track-origins=yes --leak-check=full --show-reachable=yes -v --log-file=valgrind.log ./sample
//!<


#include <stdio.h>
#include "DBCClient.h"
#include "json.h"


//!< Uncomment next line with tha valid token value
//#define TOKEN "4f558dfde45e6d0e1850da93954c91b1"
#define VERSION "2"
#define API "article"
#define URL "http://allthingsd.com/google-gets-semantic-launches-knowledge-graph-in-english-starting"

int main(int argc, const char * argv[]) {
    //!<
    //!< First of all create and init control structure
    //!<
    DBRequest* request = initDBRequest();
    
    //!<
    //!< Next, add version (currently supported version 1 and 2)
    //!<
    addDBVersion(request, VERSION);
    
    //!<
    //!< Next, add product category
    //!< currently supported product categories:
    //!< article
    //!< frontpage
    //!< product
    //!< image
    //!< classifier
    //!<
    addDBAPI(request, API);
    
    //!<
    //!< Next, add token
    //!<
    addDBToken(request, TOKEN);
    
    //!<
    //!< Next, fill up fields
    //!<
    addDBRequestField(request, "url", URL);
    
    //!<
    //!< At last, make request and take response back
    //!<
    json_object* json = diffBot(request);

    //!<
    //!< Print out result
    //!<
    printf("%s\n", json_object_to_json_string(json));

    addDBAPI(request, "image");
    json_object_put(json);
    json = diffBot(request);
    printf("\n\n%s\n", json_object_to_json_string(json));
    
    addDBAPI(request, "frontpage");
    json_object_put(json);
    json = diffBot(request);
    printf("\n\n%s\n", json_object_to_json_string(json));

    addDBAPI(request, "product");
    json_object_put(json);
    json = diffBot(request);
    printf("\n\n%s\n", json_object_to_json_string(json));

    
    //!<
    //!< Clean up resources
    //!<
    json_object_put(json);
    cleanDBRequest(request);
    return 0;
}

