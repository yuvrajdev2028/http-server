#pragma once

#include <string>
#include <unordered_map>
#include <sstream>

// The HttpRequest class parses an incoming HTTP request string (like "GET /index.html HTTP/1.1\r\n...")
// into distinct parts that are easy to use in C++.
// We hold the pieces of the request in simple string variables.
class HttpRequest {
public:
    std::string method;  // e.g., "GET" or "POST"
    std::string path;    // e.g., "/index.html"
    std::string version; // e.g., "HTTP/1.1"
    std::unordered_map<std::string, std::string> headers; // To store key-value pairs of HTTP headers
    std::string body;    // The body/payload of the request (often empty for GET requests)

    // Parses a raw HTTP string into the fields of this object.
    bool parse(const std::string& raw_request);
};
