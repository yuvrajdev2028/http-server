#include "server.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <thread>
#include <vector>

Server::Server(int port) : port(port), server_fd(-1) {
    // 1. Create the socket (IPv4, TCP)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket.\n";
        exit(EXIT_FAILURE);
    }

    // 2. Allow reusing the port (helps avoid "Address already in use" errors during quick restarts)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        std::cerr << "Failed to set socket options.\n";
        exit(EXIT_FAILURE);
    }

    // 3. Bind the socket to the port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces
    address.sin_port = htons(port);       // Convert port to network byte order

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Failed to bind to port " << port << ".\n";
        exit(EXIT_FAILURE);
    }

    // 4. Start listening for incoming connections
    if (listen(server_fd, 10) < 0) {
        std::cerr << "Failed to listen on socket.\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Server successfully started. Listening on port " << port << "...\n";
}

Server::~Server() {
    if (server_fd != -1) {
        close(server_fd);
    }
}

void Server::start() {
    while (true) {
        int addrlen = sizeof(address);
        // Default blocks until a client connects
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        
        if (client_socket < 0) {
            std::cerr << "Failed to accept client connection.\n";
            continue;
        }

        // Spawn a new thread to handle the client asynchronously
        // std::thread runs the handle_client function independently.
        std::thread t(&Server::handle_client, this, client_socket);
        t.detach(); // Detach the thread so it runs independently and cleans up after itself
    }
}

void Server::handle_client(int client_socket) {
    // Buffer to read incoming data
    char buffer[4096] = {0};
    
    // Read the request from the client into the buffer
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        // We Successfully read something
        std::string raw_request_str(buffer, bytes_read);

        // Parse the request
        HttpRequest request;
        if (request.parse(raw_request_str)) {
            std::cout << "Received " << request.method << " request for " << request.path << "\n";
            
            // Build a dynamic response based on the path
            HttpResponse response;
            if (request.path == "/") {
                response.set_status(200, "OK");
                response.set_header("Content-Type", "text/plain");
                response.set_body("Welcome to the C++ HTTP Server!");
            } else if (request.path == "/echo" && request.method == "POST") {
                response.set_status(200, "OK");
                response.set_header("Content-Type", "text/plain");
                response.set_body("You said: " + request.body);
            } else {
                response.set_status(404, "Not Found");
                response.set_header("Content-Type", "text/plain");
                response.set_body("404 - The requested resource was not found.");
            }

            // Send the response string back to the client
            std::string response_str = response.to_string();
            send(client_socket, response_str.c_str(), response_str.size(), 0);
        } else {
            std::cerr << "Failed to parse malformed HTTP request.\n";
            HttpResponse response;
            response.set_status(400, "Bad Request");
            response.set_body("400 - Bad Request");
            std::string response_str = response.to_string();
            send(client_socket, response_str.c_str(), response_str.size(), 0);
        }
    }

    // Close the connection once done
    close(client_socket);
}
