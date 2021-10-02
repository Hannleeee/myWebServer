#ifndef BASELIBS_H
#define BASELIBS_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <type_traits>

#define MAXLINE 8192
#define MAXBUF   8192
#define LISTENQ  1024 

// rio related
#define RIO_BUFSIZE 8192

typedef sockaddr SA;

typedef struct {
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
} rio_t;

ssize_t rio_writen(int fd, void *usrbuf, size_t n);     // why use void * for usrbuf ?
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
void rio_readinitb(rio_t *rp, int fd);                  // to do what ?

// socket integrated interface
int open_clientfd(char *hostnNme, char *port);
int open_listenfd(char *port);

void echo(int connfd);

bool isInt(char *c);

#endif