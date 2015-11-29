/*
  CONFIG.H
  =========

Function for Configuring the server to a port number, root directory
& implemented file extensions.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

int config_server(struct config *conf)
{
	FILE *fp;
	char *buffer = calloc(1024, sizeof(char));
	fp = fopen("/home/ruhi/git/ws.conf", "r");
	if(fp == NULL) {
		perror("Error opening file");
		return(-1);
	}

	while(fgets(buffer, 60, fp) != NULL) {
		if(!strncmp(buffer, "Listen ",7)) {
			buffer += 7;
			conf->port = atoi(buffer);
		}
		if(!strncmp(buffer, "DocumentRoot ",13)) {
			buffer += 13;
			conf->root = calloc(strlen(buffer),sizeof(char));
			strncpy(conf->root,buffer, strlen(buffer)-1);
		}
		if(!strncmp(buffer,"DirectoryIndex ",15)) {
			buffer += 15;
			conf->index_page = calloc(strlen(buffer), sizeof(char));
			strncpy(conf->index_page, buffer, strlen(buffer)-1);
		}
		if(!strncmp(buffer,".",1))
			strncat(conf->supported_types,buffer, strlen(buffer));
	}
}
