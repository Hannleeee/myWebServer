#include "baseLibs.h"

int main(int argc, char *argv[]) {
    if (argc!=3 || !isInt(argv[2])) {
        std::cerr << "[ERROR] Usage: " << argv[0] << " <host> <port>" << std::endl << "(port must be an integer)";
        return -1;
    }

    int port = std::atoi(argv[2]);
    hostent *server = gethostbyname(argv[1]);
    if (!server) {
        std::cerr << "[ERROR] No such host!\n";
        return -2;
    }
    std::cout << "[INFO] Host name has been found!\n";

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        std::cerr << "[ERROR] Socket can't be created!\n";
        return -3;
    }
    std::cout << "[INFO] Socket has been created!\n";

    sockaddr_in server_addr;
    std::memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (server->h_addr_list[0]) std::memcpy((char *)server->h_addr_list[0], (char *)&server_addr.sin_addr, server->h_length);
    else {
        std::cerr << "[ERROR] No valid address for this hostname!\n";
        return -5;
    }
    if (connect(sock_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection cannot be established!\n";
        return -6;
    }
    std::cout << "[INFO] Connection established.\n";

    char buf[4096] = "test!";
    std::string temp;
    do {
        std::memset(buf, 0, 4096);
        std::cout << "> ";
        std::getline(std::cin, temp, '\n');
        std::strcpy(buf, temp.c_str());

        if (!strlen(buf)) continue;
        else if (!strcmp(buf, "quit")) break;

        int bytes_send = send(sock_fd, &buf, (size_t)strlen(buf), 0);
        if (bytes_send<0) {
            std::cerr << "[ERROR] Message can't be send!\n";
            break;
        }

        int bytes_recv = recv(sock_fd, &buf, 4096, 0);
        if (bytes_recv<0) {
            std::cerr << "[ERROR] Message can't be received!\n";
        }
        else if (bytes_recv==0) {
            std::cout << "[INFO] Server is disconnected!\n";
        }
        else {
            std::cout << "server> " << std::string(buf, 0, bytes_recv) << "\n";
        }
    } while (true);

    close(sock_fd);     // inluded in unistd.h
    std::cout <<"[INFO] Socket is closed!\n";

    return 0;
}