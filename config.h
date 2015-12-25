/* $begin config.h */
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <netinet/in.h>
#include <sys/socket.h>

#define LISTENQ  1024  /* second argument to listen() */

typedef struct sockaddr SA;
struct config{
	int port;
	char *root;
	char *index_page;
	char supported_types[2048];
};

int config_server(struct config *conf);
int open_listenfd(int port);

#endif /*__CONFIG_H_*/
