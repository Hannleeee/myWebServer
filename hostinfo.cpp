#include "./echoServer/baseLibs.h"

int main (int argc, char *argv[]) {
    struct addrinfo *p, *listp, hints;
    char buf[MAXLINE];

    if (argc!=2) {
        std::cerr << "Usage : " << argv[0] << " <domain name>\n";
        return -1;
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int rc;
    if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
        std::cerr << "Getaddrinfo error : " << gai_strerror(rc) << "\n";
        return -2;
    }

    int flags = NI_NUMERICHOST;
    for (p=listp; p; p=p->ai_next) {
        getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flags);
        std::cout << buf << "\n";
    }
    freeaddrinfo(listp);

    return 0;
}