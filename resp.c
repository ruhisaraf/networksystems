/*
RESP.C
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "config.h"
#include "req.h"
#include "resp.h"

int resp_error(int clientfd, struct Req *http_req)
{
	char err_msg[100] = {0};
	char buffer[1024] = {0};

	switch(http_req->status) {
		case 400:
			if(http_req->method == NULL)
				strcpy(err_msg,"Invalid Method");
			else if(http_req->version == 0)
				strcpy(err_msg,"Invalid Method");
			else
				strcpy(err_msg,"Invalid URL");
			break;
		case 404:
			strcpy(err_msg,"File Not Found");
			break;
		case 501:
			strcpy(err_msg,"Not Implemented");
			break;
		default:
			strcpy(err_msg,"Internal Server Error");
	}
	
	sprintf(buffer,"HTTP/%.1f %d %s\n\n",http_req->version,http_req->status, err_msg);
	write(clientfd, buffer, strlen(buffer));
	return 0;
}

int resp_req(int clientfd, struct Req *http_req, struct config *conf)
{
	struct stat fileStat;
	char header[1024];
	char buffer[1024];
	char type[20];
	int x, f_size;
	int fp;
	char *endptr;

	char *ext = strrchr(http_req->url, '.');
	char *ct = strstr(conf->supported_types,ext);
    ct += strlen(ext) + 1;
	endptr = strchr(ct,'\n');
    int length = strlen(ct) - strlen(endptr);
	
	memcpy(type, ct, length);

	stat(http_req->url, &fileStat);
	f_size = fileStat.st_size;

	if(fp = open(http_req->url, O_RDONLY)) {		
		sprintf(header,"HTTP/1.0 200 OK\nContent-type: %s\nContent-length: %d\n\n",type,f_size);

		write(clientfd, header, strlen(header));
		while((x = read(fp, buffer, 1024)) > 0)
			write(clientfd,buffer,x);
		close(fp);
	}
}
