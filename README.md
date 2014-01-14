# Diffbot API C Client Library

## Description: 

**This is a simple client for the Diffbot API in the C language**

## Prerequisites

1. _libcurl_ [http://curl.haxx.se/libcurl](http://curl.haxx.se/libcurl)
2. _json-c_ [https://github.com/jehiah/json-c](https://github.com/jehiah/json-c)

## Build

<code>
make clean all
</code>

**wiil build the library itself with the demo application**

**or**

<code>
make lib
</code>

**to build library**

<code>
make demo
</code>

**to build demo application**

## Deploying

1. **copy**
	<code>
	DBCClient.h
	</code> 
	**header file with the library file**
 	<code>
	libdiffboot.a
	</code> 
	**to your work directory.**
	
	**Assume:**

	<code>
	$WORKDIR
	</code> 
	**- your working directory where source files are located**

	<code>
	$LIBDIR
	</code> 
	**- the directory where you builded the library**

	<code> 
	cp $LIBDIR/DBCClient.h $WORKDIR
	</code>

	<code> 
	cp $LIBDIR/libdiffboot.a $WORKDIR
	</code>

2. **Include header file to your source file**

	<code>
	\#include "DBCClient.h"
	</code>

3. **Add next lines to your Makefile**

	<code>
	CFLAGS = $(shell curl-config --cflags) $(shell pkg-config --cflags json)
	</code>

	<code>
	LDFLAGS = $(shell curl-config --libs) $(shell pkg-config --libs json) -L$($WORKDIR) -ldiffboot
	</code>

## Examples

**Here are example of basic using of the DiffBot C library**
	
**_First of all create and init control structure_**

<!-- language: c++ -->

<code>
    DBRequest* request = initDBRequest();
</code>

**_Next, add version (currently supported version 1 and 2)_**

<code>
    addDBVersion(request, "2");
</code>

  **_Next, add product API_**

<code>
    addDBAPI(request, "article");
</code>

  **_currently supported product API:_**

  * _article_
  * _frontpage_
  * _product_
  * _image_
  * _classifier_
  
**_Next, add token_**

<code>
    addDBToken(request, TOKEN);
</code>

**NOTE:	TOKEN 	must be a valid developer's token**

**_Next, fill up fields_**

<code>
    addDBRequestField(request, "url", URL);
</code>

**NOTE: URL must be a valid url, in example http://allthingsd.com/google-gets-semantic-launches-knowledge-graph-in-english-starting**

**_At last, make request and take response back_**

<code>
    json_object* json = diffBot(request);
</code>

**_Print out result_**

<code>
    printf("%s\n", json_object_to_json_string(json));
</code>

**_At final do cleanup_**
    
<code>
	json_object_put(json);

	cleanDBRequest(request);
</code>


## License

**That library are available under MIT license**