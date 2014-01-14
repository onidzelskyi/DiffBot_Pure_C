# _Diffbot API C Client Library_

_Description: This is a simple client for the Diffbot API in the C language_

## Project Setup

_Prerequisites_

1. _libcurl (http://curl.haxx.se/libcurl/)_
2. _json-c (https://github.com/jehiah/json-c)_

make

## Deploying

### _How to setup the deployment environment_

_CFLAGS = $(shell curl-config --cflags) $(shell pkg-config --cflags json)_

_LDFLAGS = $(shell curl-config --libs) $(shell pkg-config --libs json)_

## _Examples_

_Here are example of basic using of the DiffBot C library_

 _First of all create and init control structure_

    DBRequest* request = initDBRequest();

_Next, add version (currently supported version 1 and 2)_

    addDBVersion(request, "2");

  _Next, add product category_

    addDBAPI(request, "article");

  _currently supported product categories:_
  * article
  * frontpage
  * product
  * image
  * classifier
  

    addDBAPI(request, "article");

_Next, add token_

    addDBToken(request, <TOKEN>);

  Next, fill up fields
  _<URL> is valid url, in example http://allthingsd.com/google-gets-semantic-launches-knowledge-graph-in-english-starting_

    addDBRequestField(request, "url", <URL>);

  _At last, make request and take response back_

    json_object* json = diffBot(request);

  _Print out result_

    printf("%s\n", json_object_to_json_string(json));

  _TODO cleanup_
    
    json_object_put(json);
    cleanDBRequest(request);


## License

_That library are available under MIT license_