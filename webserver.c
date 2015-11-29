/*
Simple Web Server
Course: Network Systems
*/

#include "csapp.h"
#include "sbuf.h"
#include "config.h"
#include "req.h"
#include "resp.h"

#define NTHREADS 200
#define SBUFSIZE 200
#define MAX_LEN_REQ 4096

/* Global Variables */
sbuf_t sbuf;
struct config conf;
void *thread(void *vargp);

int main()
{

	int listenfd, clientfd;
        socklen_t socket_length;
	struct sockaddr_in clientaddr;
	int i;
	config_server(&conf);

	pthread_t tid;

	socket_length = sizeof(clientaddr);

	sbuf_init(&sbuf, SBUFSIZE);
	for (i = 0; i < NTHREADS; i++)
		Pthread_create(&tid, NULL, thread, NULL);

	listenfd = Open_listenfd(conf.port);

	while (1)
	{
		clientfd = Accept(listenfd, (SA *)&clientaddr, &socket_length);
		sbuf_insert(&sbuf, clientfd);
	}
}

void *thread(void *vargp)
{
	Pthread_detach(pthread_self());
	while (1)
	{
		int clientfd = sbuf_remove(&sbuf);
		service_requests(clientfd);
	}
}

int service_requests(int clientfd)
{

	struct Req http_req;
	init_req(&http_req);

	if(get_req(clientfd, &http_req, &conf) != -1)
		resp_req(clientfd, &http_req, &conf);
	

	free_req(&http_req);
	close(clientfd);

}

int get_req(int clientfd, struct Req *http_req, struct config *conf)
{

	char buffer[MAX_LEN_REQ];
	int retval;

	retval = recv(clientfd, buffer, sizeof(buffer), 0);
	if(parse_request(buffer, http_req, conf) < 0)
	{
		resp_error(clientfd, http_req);
		return -1;
	}

	return 0;

}
