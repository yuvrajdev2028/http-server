cmake_minimum_required(VERSION 3.14)

project(http-server)

# Set C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find threading library (required for std::thread)
find_package(Threads REQUIRED)

# Add the main library source files
set(SOURCE_FILES 
    src/server.cpp 
    src/http_request.cpp 
    src/http_response.cpp
)

# Main Executable
add_executable(http-server src/main.cpp ${SOURCE_FILES})
target_include_directories(http-server PRIVATE src)
target_link_libraries(http-server PRIVATE Threads::Threads)

# Fetch GoogleTest for unit testing
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add test executable
add_executable(http-server-tests 
    tests/test_http_request.cpp 
    tests/test_http_response.cpp
    ${SOURCE_FILES}
)
target_include_directories(http-server-tests PRIVATE src)
target_link_libraries(http-server-tests PRIVATE gtest_main Threads::Threads)

include(GoogleTest)
gtest_discover_tests(http-server-tests)