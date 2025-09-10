cmake_minimum_required(VERSION 3.10)
project(TodoServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Threads REQUIRED)

# Add executable
add_executable(todo_server 
    src/main.cpp
    src/http_server.cpp
    src/todo_manager.cpp
    src/websocket_server.cpp
)

target_include_directories(todo_server PRIVATE include)
target_link_libraries(todo_server Threads::Threads)

# Compiler-specific options
if(MSVC)
    target_compile_options(todo_server PRIVATE /W4)
else()
    target_compile_options(todo_server PRIVATE -Wall -Wextra -pedantic)
endif()