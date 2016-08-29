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
	return 0;
}

int open_listenfd(int port) 
{
    int listenfd, optval=1;
    struct sockaddr_in serveraddr;
  
    /* Create a socket descriptor */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	return -1;
 
    /* Eliminates "Address already in use" error from bind. */
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
		   (const void *)&optval , sizeof(int)) < 0)
	return -1;

    /* Listenfd will be an endpoint for all requests to port
       on any IP address for this host */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port = htons((unsigned short)port); 
    if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0)
	return -1;

    /* Make it a listening socket ready to accept connection requests */
    if (listen(listenfd, LISTENQ) < 0)
	return -1;
    return listenfd;
}
