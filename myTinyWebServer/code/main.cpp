#include <unistd.h>
#include "server/webserver.h"

int main() {
    WebServer server(
        9876, 3, 60000, false,
        3306, "root", "1101", "mydb",
        12, 6, true, 1, 1024);
    server.Start();
}