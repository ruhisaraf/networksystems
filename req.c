/*
    REQ.C
    =========

Function for Parsing the user request and storing
int a struct.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "req.h"

void init_req(struct Req *http_req)
{
	http_req->method = NULL;
	http_req->url = NULL;
	http_req->version = 0;
	http_req->conn_state = 0;
	http_req->status = 0;
}

void free_req(struct Req *http_req)
{
	if ( http_req->method )
		free(http_req->method);
	if ( http_req->url )
		free(http_req->url);
}

int parse_request( char *buffer, struct Req *http_req, struct config *conf)
{

	char *endptr;
	int len;

	/*Save Method */
	if(!strncmp(buffer, "GET ",4)) {
		http_req->method = calloc(4, sizeof(char));
		strncpy(http_req->method, buffer, 3);
		buffer += 4;
	}
	else
		http_req->status = 400;

	/*Save URL */
	while(*buffer && isspace(*buffer))
		buffer++;

	endptr = strchr(buffer, ' ');
	if(endptr == NULL)
		http_req->status = 400;
	else {
		len = endptr - buffer;
		http_req->url = calloc(strlen(conf->root) + len + 1, sizeof(char));
		strncpy(http_req->url,conf->root,strlen(conf->root));
		strncat(http_req->url, buffer, len);
		if( (len==1) && (*buffer == '/'))
			strcat(http_req->url,"index.html");
		buffer += len + 1;
	}
	
	/*Save Version */
	if( !strncmp(buffer, "HTTP/1.0",8))
		http_req->version = 1.0;
	else if( !strncmp(buffer, "HTTP/1.1",8))
		http_req->version = 1.1;
	else
		http_req->status = 400;

	if(http_req->status == 400)
		return -1;

	/*Save Connection: State */
	endptr = strstr(buffer, "Connection: keep-alive");
	if(endptr == NULL)
		http_req->conn_state = 0;
	else
		http_req->conn_state = 1;
	
	/*Check if file exists */
	if(access(http_req->url, F_OK) == -1) {
		http_req->status = 404;
		return -1;
	}

	/*Check file type */
	endptr = strrchr(http_req->url, '.');
	if((endptr != NULL) && (strstr(conf->supported_types,endptr) != NULL))
		http_req->status = 200;
	else {
		http_req->status = 501;
		return -1;
	}
		

	return 0;
}
