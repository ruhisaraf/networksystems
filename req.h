/* $begin req.h */
#ifndef __REQ_H__
#define __REQ_H__

struct Req{
	char *method;
	char *url;
	float version;
	int conn_state;
	int status;
};

void init_req(struct Req *http_req);
int get_req(int clientfd, struct Req *http_req, struct config *conf);
void free_req(struct Req *http_req);
int parse_request( char *buffer, struct Req *http_req, struct config *conf);

#endif /*__REQ_H_*/
