#include "baseLibs.h"

int main(int argc, char *argv[]) {
    if (argc!=2 || !isInt(argv[1])) {
        std::cerr << "[ERROR] Usage: " << argv[0] << " <port>" << std::endl << "(port must be an integer)\n";
        return -1;
    }
    
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];;

    listenfd = open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
        getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        echo(connfd);
        close(connfd);
    }

    return 0;
}