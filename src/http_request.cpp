#include "http_request.hpp"
#include <sstream>

bool HttpRequest::parse(const std::string& raw_request) {
    std::istringstream stream(raw_request);
    std::string line;

    // 1. Parsing the Request Line (e.g., "GET / HTTP/1.1")
    if (std::getline(stream, line)) {
        // Remove trailing carriage return (\r) if it exists, typical of HTTP lines
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        std::istringstream line_stream(line);
        // Extract method, path, and version separated by spaces
        line_stream >> method >> path >> version;
        
        // If any of these are empty, parsing the request line failed
        if (method.empty() || path.empty() || version.empty()) {
            return false;
        }
    } else {
        return false;
    }

    // 2. Parsing the Headers (Key: Value)
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // An empty line indicates the end of headers
        if (line.empty()) {
            break;
        }

        // Find where the colon separates the Header-Name and Header-Value
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            // Skip the colon and leading spaces of the value
            size_t value_start = line.find_first_not_of(" \t", colon_pos + 1);
            std::string value = "";
            if (value_start != std::string::npos) {
                value = line.substr(value_start);
            }
            headers[key] = value;
        }
    }

    // 3. The remainder of the stream is the Body
    std::ostringstream body_sstream;
    body_sstream << stream.rdbuf();
    body = body_sstream.str();

    return true;
}
