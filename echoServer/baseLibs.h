#ifndef BASELIBS_H
#define BASELIBS_H
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

// #define MAXLINE 8192

// // rio related
// #define RIO_BUFSIZE 8192

// typedef struct {
//     int rio_fd;
//     int rio_cnt;
//     char *rio_bufptr;
//     char rio_buf[RIO_BUFSIZE];
// } rio_t;

// ssize_t rio_writen(int fd, void *usrbuf, size_t n);     // why use void * for usrbuf ?
// ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
// void rio_readinitb(rio_t *rp, int fd);                  // to do what ?

// // socket integrated interface
// int Open_clientfd(char *hostnNme, char *port);
// int Open_listenfd(char *port);

bool isInt(char *c) {
    while (*c != '\0') {
        if (*c<'0' || *c>'9') return false;
        ++c;
    }
    return true;
}

#endif