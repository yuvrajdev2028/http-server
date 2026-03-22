# C++ HTTP Server

A complete, end-to-end HTTP Server written from scratch in C++23. It demonstrates core backend mechanisms such as socket creation, binding, listening, multi-threading, and parsing HTTP requests and responses.

## Features
- **Multi-threaded**: Spawns a new `std::thread` for each incoming client connection.
- **Request Parsing**: Manual parsing of HTTP/1.1 methods, paths, and headers.
- **Response Building**: Easily construct statuses, headers, and bodies dynamically.
- **Automated Testing**: Integrated with GoogleTest for unit testing HTTP component layers.

## Documentation
Check the `docs/` folder for technical breakdowns:
- `docs/architecture.txt`: ASCII diagram mapping out socket listener and connection thread separation.
- `docs/workflow.txt`: ASCII diagram showing the lifetime of a single request from `accept()` to `send()`.
- `docs/decisions.md`: A log of key technical decisions and architectural alternatives that were weighed.

## Prerequisites
- A C++23 compatible compiler (e.g., `g++` 11+ or `clang++` 14+)
- `CMake` (3.14+)
- `make`

## How to Build and Run

1. **Clone and enter the directory**:
   ```bash
   cd http-server
   ```

2. **Configure with CMake**:
   ```bash
   mkdir -p build && cd build
   cmake ..
   ```

3. **Build the project**:
   ```bash
   make
   ```

4. **Run the server**:
   ```bash
   ./http-server
   ```
   *The server will start listening on port 8080.*

## How to Test

Once the server is running, you can test it locally using `curl`:

**Basic GET Request**
```bash
curl -v http://localhost:8080/
```

**Echo POST Request**
```bash
curl -v -X POST -d "Hello C++" http://localhost:8080/echo
```

**Invalid Route**
```bash
curl -v http://localhost:8080/unknown-path
```

## Running Unit Tests
Unit tests are bundled via GoogleTest. To execute them:
```bash
cd build
ctest --output-on-failure
```
