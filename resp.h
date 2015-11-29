/* $begin resp.h */
#ifndef __RESP_H__
#define __RESP_H__

int resp_error(int clientfd, struct Req *http_req);
int resp_req(int clientfd, struct Req *http_req, struct config *conf);

#endif /*__RESP_H_*/
