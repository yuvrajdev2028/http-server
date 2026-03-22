# Technical Decisions Document

This document lists key technical decisions made during the creation of this C++ HTTP Server.

## 1. Concurrency Model: std::thread vs thread pool vs async/epoll

**Decision Made**: `std::thread` per connection.

**Reasoning**:

- The primary goal of this codebase is to remain highly readable and approachable.
- Spawning a new `std::thread` per client connection introduces multi-threading concepts very intuitively (one connection = one thread function).
- Code overhead is isolated to a single line `std::thread t(..., client_socket); t.detach();`.

**Alternatives Weighed**:

1. **Single-threaded (blocking)**: This is the easiest, but it prevents the server from handling more than 1 request simultaneously. It's too simplistic for a realistic end-to-end web server.
2. **epoll / kqueue (Event-driven / Async I/O)**: This is the industry standard for high-performance servers (e.g., NGINX, Node.js). However, managing non-blocking I/O callbacks or asynchronous state machines adds significant mental overhead and complexity.
3. **Thread Pool**: More robust than "thread per connection" as it prevents resource exhaustion under heavy load. We chose not to do this to avoid introducing complex thread synchronization (mutexes, condition variables, tasks queues) into the core logic, which distracts from the HTTP server logic itself.

## 2. HTTP Parsing Strategy: Standard Library Streams vs Hand-rolled state machine vs Library

**Decision Made**: C++ Standard Library `std::istringstream` and `std::getline`.

**Reasoning**:

- Makes manual string parsing very straightforward and safe.
- `std::getline` easily handles line-by-line reading, and `std::istringstream` trivially separates space-delimited text (like `GET / HTTP/1.1`).
- Avoids pointer arithmetic or complex C-style string manipulations (`strtok`, `char*`), keeping the memory model simple and reducing the chance of segmentation faults.

**Alternatives Weighed**:

1. **Hand-rolled State Machine / Regex**: Checking character by character is extremely efficient but verbose and hard to maintain without advanced knowledge. Regex could be slow, and C++ regex syntax is famously clunky.
2. **External parsing libraries (e.g., picohttpparser or http-parser)**: Very fast and reliable. However, hiding the parsing inside a library obfuscates how HTTP works under the hood in the context of this implementation.

## 3. Testing Framework: GoogleTest (FetchContent) vs Catch2 vs Custom macros

**Decision Made**: GoogleTest fetched automatically via CMake `FetchContent`.

**Reasoning**:

- GoogleTest is the most widely recognized testing framework in the C++ industry.
- `FetchContent` ensures that the user does not have to install anything manually (like `apt-get install libgtest-dev`), just run CMake and it works out of the box.

**Alternatives Weighed**:

1. **Catch2**: Also an excellent, modern, header-only (or standard) testing library. GTest was selected due to broader industry ubiquity.
2. **Custom assertions / simple `main()` test runner**: Writing our own simple test functions is easiest to start with, but it breaks down quickly as tests grow. Exposing the user to a real testing framework gives a better sense of standard C++ project structures.
