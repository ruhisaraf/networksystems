/*
Simple Web Server
Course: Network Systems
*/

//#include "csapp.h"
#include "config.h"
#include "req.h"
#include "resp.h"
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h> 
#include <errno.h>


#define MAX_LEN_REQ 4096

/* Global Variables */

struct config conf;
void sigchld_handler(int s);

int main()
{

	int listenfd, clientfd;
    	socklen_t socket_length;
	struct sockaddr_in clientaddr;
	int i;
	struct sigaction sa;

	config_server(&conf);

	socket_length = sizeof(clientaddr);

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
		perror("ERROR: Signal Action failed\n");

	listenfd = open_listenfd(conf.port);

	while (1) {
		clientfd = accept(listenfd, (SA *)&clientaddr, &socket_length);

		if(fork() == 0) {
			close(listenfd);
			service_requests(clientfd);
			return 0;
		}

		close(clientfd);

		waitpid(-1, NULL, WNOHANG);
	}
}
void sigchld_handler(int s)
{
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

int service_requests(int clientfd)
{
	fd_set master;
	fd_set readfds;
	struct timeval timeout;
	int sel;
	int fdmax;

	fdmax = clientfd;
	FD_ZERO(&master);
	FD_ZERO(&readfds);
	FD_SET(clientfd, &master);

	while(1) {

		readfds = master;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;

		sel = select(fdmax+1,&readfds, NULL, NULL, &timeout);

		if(sel < 0) {
			perror("Select Error: ");
			break;
		}
		if(sel == 0) {
			break;
		}
		if(sel > 0) {
			if(FD_ISSET(clientfd, &readfds)) {
				struct Req http_req;
				init_req(&http_req);

				if(get_req(clientfd, &http_req, &conf) != -1) {
					resp_req(clientfd, &http_req, &conf);
					free_req(&http_req);
				}

				else
				    break;
			}
		}
	}
	
	close(clientfd);

}

int get_req(int clientfd, struct Req *http_req, struct config *conf)
{

	char buffer[MAX_LEN_REQ];
	int retval;

	retval = recv(clientfd, buffer, sizeof(buffer), 0);
	if(retval == 0)
		return -1;

	if(parse_request(buffer, http_req, conf) < 0) {
		resp_error(clientfd, http_req);
		return -1;
	}

	return 0;

}
