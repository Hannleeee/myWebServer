#include <baseLibs.h>

int main(int argc, char *argv[]) {
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <host> <port>\n";
        std::exit(0);
    }
    host = argv[1]; port = argv[2];

    clientfd = Open_clientfd(host, port);
    rio_readinitb(&rio, clientfd);

    while (fgets(buf, MAXLINE, std::cin) != nullptr) {
        rio_writen(clientfd, buf, strlen(buf));
        rio_readlineb(&rio, buf, MAXLINE);
        fputs(buf, std::cout);
    }
}