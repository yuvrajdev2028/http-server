#include "server.hpp"

int main() {
    // Create a server listening on port 8080
    Server server(8080);
    
    // Start listening for connections forever
    server.start();

    return 0;
}
