/**
 * @file DBCClient.h
 * @author Oleksii Nidzelskyi <alexey.education@gmail.com>
 * @copyright Copyright &copy; 2014 Ukraine
 * @license MIT
 * @since 0.1
 */

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

/** @brief Create and inialize control structure DBRequest
 *  @return pointer to newly created control structure DBRequest
 *  or null if there not enought memory to allocate it.
 */
DBRequest* initDBRequest();


/** @brief clean memory allocated by the DBRequest control structure
 *  @param request Pointer to the DBRequest control structure.
 */
void cleanDBRequest(DBRequest* request);


/** @brief Initialize version API of the DBRequest control structure
 *  @param request Pointer to the DBRequest control structure.
 *  @param version Pointer to the null-terminated C-string
 */
void addDBVersion(DBRequest* request, const char* version);


/** @brief Initialize API name of the DBRequest control structure
 *  @param request Pointer to the DBRequest control structure.
 *  @param api Pointer to the null-terminated C-string
 */
void addDBAPI(DBRequest* request, const char* api);


/** @brief Initialize token value of the DBRequest control structure
 *  @param request Pointer to the DBRequest control structure.
 *  @param token Pointer to the null-terminated C-string
 */
void addDBToken(DBRequest* request, const char* token);


/** @brief Initialize key and value pair of the DBRequest control structure
 *  @param request Pointer to the DBRequest control structure.
 *  @param name Pointer to the null-terminated C-string
 *  @param value Pointer to the null-terminated C-string
 */
void addDBRequestField(DBRequest* request, const char* name, const char* value);


/** @brief Perform client API request
 *  @param request Pointer to the DBRequest control structure.
 *  @return pointer to newly created json object
 */
json_object* diffBot(DBRequest* request);




#endif
