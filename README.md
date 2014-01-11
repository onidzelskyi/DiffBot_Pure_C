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

 _First of all define control structure_

    DBRequest request;

  _Then, initialize it_

    `initDBRequest(&request);`

  _Next, add version (currently supported version 1 and 2)_

    addDBVersion(&request, "2");

  _Next, add product category_
    currently supported product categories:
    article
    frontpage
    product
    image
    classifier

    <code>
    addDBAPI(&request, "article");
    </code>

    Next, add token

    <code>
    addDBToken(&request, "4f558dfde45e6d0e1850da93954c91b1");
    </code>

    Next, fill up fields

    <code>
    addDBRequestField(&request, "url", "http://allthingsd.com/google-gets-semantic-launches-knowledge-graph-in-english-starting");
    </code>

    At last, make request and take response back

    <code>
    json_object* json = diffBot(&request);
    </code>

    Print out result

    <code>
    printf("%s\n", json_object_to_json_string(json));
    </code>


## License

_That library are available under MIT license_