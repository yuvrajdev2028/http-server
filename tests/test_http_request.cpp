#include "http_request.hpp"
#include <gtest/gtest.h>

TEST(HttpRequestTest, ParseValidGetRequest) {
    HttpRequest req;
    std::string raw = "GET /index.html HTTP/1.1\r\nHost: localhost\r\nUser-Agent: curl/7.68.0\r\n\r\n";
    
    EXPECT_TRUE(req.parse(raw));
    EXPECT_EQ(req.method, "GET");
    EXPECT_EQ(req.path, "/index.html");
    EXPECT_EQ(req.version, "HTTP/1.1");
    EXPECT_EQ(req.headers["Host"], "localhost");
    EXPECT_EQ(req.headers["User-Agent"], "curl/7.68.0");
    EXPECT_TRUE(req.body.empty());
}

TEST(HttpRequestTest, ParseValidPostRequestWithBody) {
    HttpRequest req;
    std::string raw = "POST /echo HTTP/1.1\r\nContent-Length: 11\r\n\r\nHello World";
    
    EXPECT_TRUE(req.parse(raw));
    EXPECT_EQ(req.method, "POST");
    EXPECT_EQ(req.path, "/echo");
    EXPECT_EQ(req.version, "HTTP/1.1");
    EXPECT_EQ(req.headers["Content-Length"], "11");
    EXPECT_EQ(req.body, "Hello World");
}

TEST(HttpRequestTest, ParseMalformedRequest) {
    HttpRequest req;
    std::string raw = "INVALID_REQUEST_LINE\r\n\r\n";
    
    EXPECT_FALSE(req.parse(raw));
}
