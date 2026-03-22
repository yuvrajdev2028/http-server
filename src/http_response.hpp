#pragma once

#include <string>
#include <unordered_map>

// The HttpResponse class encapsulates the data needed to send a response back to the client.
// It helps in building a clean text response that conforms to the HTTP protocol.
class HttpResponse {
public:
    std::string version = "HTTP/1.1";
    int status_code = 200;
    std::string status_message = "OK";
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    // Helper functions to set details easily
    void set_status(int code, const std::string& message);
    void set_header(const std::string& key, const std::string& value);
    void set_body(const std::string& new_body);

    // Converts the current state of this object into a formatted HTTP response string
    // ready to be sent over the socket.
    std::string to_string() const;
};
