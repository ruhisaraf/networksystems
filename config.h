/* $begin config.h */
#ifndef __CONFIG_H__
#define __CONFIG_H__

struct config{
	int port;
	char *root;
	char *index_page;
	char supported_types[2048];
};

int config_server(struct config *conf);

#endif /*__CONFIG_H_*/
