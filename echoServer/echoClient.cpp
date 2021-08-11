#include "baseLibs.h"

int main(int argc, char *argv[]) {
    if (argc!=3 || !isInt(argv[2])) {
        std::cerr << "[ERROR] Usage: " << argv[0] << " <host> <port>" << std::endl;
        return -1;
    }

    char *host, *port, buf[MAXLINE];
    int clientfd;
    rio_t rio;

    host = argv[1];
    port = argv[2];

    clientfd = open_clientfd(host, port);
    rio_readinitb(&rio, clientfd);
    
    while (fgets(buf, MAXLINE, stdin)) {
        rio_writen(clientfd, buf, strlen(buf));
        rio_readlineb(&rio, buf, MAXLINE);
        fputs(buf, stdout);
    }
    close(clientfd);

    return 0;
}