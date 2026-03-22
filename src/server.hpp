#pragma once

#include <string>
#include <netinet/in.h>

// The Server class encapsulates the core logic of our HTTP server.
// It handles creating a socket, listening on a port, and accepting connections.
class Server {
public:
    // Takes the port number to listen on
    Server(int port);
    ~Server();

    // Starts the server to listen for incoming connections indefinitely
    void start();

private:
    int port;
    int server_fd; // File descriptor for the server socket
    sockaddr_in address;

    // Handles a single client connection in a separate thread
    void handle_client(int client_socket);
};
