#include "http_response.hpp"
#include <gtest/gtest.h>

TEST(HttpResponseTest, DefaultInitialization) {
    HttpResponse res;
    EXPECT_EQ(res.status_code, 200);
    EXPECT_EQ(res.status_message, "OK");
    EXPECT_EQ(res.version, "HTTP/1.1");
}

TEST(HttpResponseTest, GenerateOkResponse) {
    HttpResponse res;
    res.set_status(200, "OK");
    res.set_header("Content-Type", "text/plain");
    res.set_body("Hello!");
    
    std::string expected = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 6\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello!";
    
    std::string generated = res.to_string();
    
    // Check if the generated string contains the correct bits
    // Since headers are unordered map, their order might vary
    EXPECT_NE(generated.find("HTTP/1.1 200 OK\r\n"), std::string::npos);
    EXPECT_NE(generated.find("Content-Type: text/plain\r\n"), std::string::npos);
    EXPECT_NE(generated.find("Content-Length: 6\r\n"), std::string::npos);
    EXPECT_NE(generated.find("\r\n\r\nHello!"), std::string::npos);
}

TEST(HttpResponseTest, GenerateNotFoundResponse) {
    HttpResponse res;
    res.set_status(404, "Not Found");
    res.set_body("Resource not found");
    
    std::string generated = res.to_string();
    
    EXPECT_NE(generated.find("HTTP/1.1 404 Not Found\r\n"), std::string::npos);
    EXPECT_NE(generated.find("Content-Length: 18\r\n"), std::string::npos);
    EXPECT_NE(generated.find("\r\n\r\nResource not found"), std::string::npos);
}
