#include "http_response.hpp"
#include <sstream>

void HttpResponse::set_status(int code, const std::string& message) {
    status_code = code;
    status_message = message;
}

void HttpResponse::set_header(const std::string& key, const std::string& value) {
    headers[key] = value;
}

void HttpResponse::set_body(const std::string& new_body) {
    body = new_body;
    // Automatically set Content-Length header to keep HTTP compliant
    headers["Content-Length"] = std::to_string(body.size());
}

std::string HttpResponse::to_string() const {
    std::ostringstream stream;

    // 1. the Status Line
    stream << version << " " << status_code << " " << status_message << "\r\n";

    // 2. the Headers
    for (const auto& [key, value] : headers) {
        stream << key << ": " << value << "\r\n";
    }

    // 3. A blank line separates headers from the body
    stream << "\r\n";

    // 4. the Body itself
    stream << body;

    return stream.str();
}
