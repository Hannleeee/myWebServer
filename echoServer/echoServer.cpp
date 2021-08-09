#include "baseLibs.h"

int main(int argc, char *argv[]) {
    if (argc!=2 || !isInt(argv[1])) {
        std::cerr << "[ERROR] Usage: " << argv[0] << " <port>" << std::endl << "(port must be an integer)\n";
        return -1;
    }
    
    int sock_listener = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_listener<0) {
        std::cerr << "[ERROR] Socket can't be created!\n";
        return -2;
    }
    std::cout << "[INFO] Socket has been created!\n";

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::atoi(argv[1]));
    inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);

    char buf[INET_ADDRSTRLEN];

    // Bind
    if (bind(sock_listener, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "[ERROR] Created socket can't be bind to ( "
                    << inet_ntop(AF_INET, &server_addr.sin_addr, buf, INET_ADDRSTRLEN)
                    << ":" << ntohs(server_addr.sin_port) << ")\n";
        return -3;
    }
    std::cout << "[INFO] Sock is binded to ("
                <<inet_ntop(AF_INET, &server_addr.sin_addr, buf, INET_ADDRSTRLEN)
                << ":" << ntohs(server_addr.sin_port) << ")\n";
    
    if (listen(sock_listener, SOMAXCONN) < 0) {
        std::cerr << "[ERROR] Socket can't be switched to liseten mode!\n";
        return -4;
    }
    std::cout << "[INFO] Socket is listening now!\n";

    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int sock_client;
    if ((sock_client=accept(sock_listener, (sockaddr *)&client_addr, &client_addr_size)) < 0) {
        std::cerr << "[ERROR] Connection can't be accepted for some reason!\n";
        return -5;
    }
    std::cout << "[INFO] A connection is accepted now!\n";

    close (sock_listener);
    std::cout << "[INFO] Main listener is closed!\n";

    // Get name info
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    if (getnameinfo(
            (sockaddr*)&client_addr, client_addr_size,
            host, NI_MAXHOST,
            svc, NI_MAXSERV, 0) != 0) {
        std::cout << "[INFO] Client: (" << inet_ntop(AF_INET, &client_addr.sin_addr, buf, INET_ADDRSTRLEN)
                << ":" << ntohs(client_addr.sin_port) << ")\n"; 
    } else {
        std::cout << "[INFO] Client: (host: " << host << ", service: " << svc << ")\n";
    }

    
    char msg_buf[4096];
    int bytes;
    // While receiving - display & echo msg
    while (true) {
        bytes = recv(sock_client, &msg_buf, 4096, 0);
        // Check how many bytes recieved
        // If there is no data, it means server is disconnected
        if (bytes == 0) {
        std::cout << "[INFO] Client is disconnected.\n";
        break;
        }
        // If something gone wrong
        else if (bytes < 0) {
        std::cerr << "[ERROR] Something went wrong while receiving data!.\n";
        break;
        }
        // If there is some bytes
        else {
        // Print message
        std::cout << "client> " << std::string(msg_buf, 0, bytes) << "\n";
        // Resend the same message
        if (send(sock_client, &msg_buf, bytes, 0) < 0) {
            std::cerr << "[ERROR] Message cannot be send, exiting...\n";
            break;
        }
        }

    }

    // Close client socket
    close(sock_client);
    std::cout << "[INFO] Client socket is closed.\n";

    return 0;
}