#include "http_server.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <regex>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif

HttpServer::HttpServer(int port) : port_(port), running_(false) {}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::setPostHandler(std::function<std::string(const std::string&)> handler) {
    post_handler_ = handler;
}

void HttpServer::setPutHandler(std::function<std::string(int)> handler) {
    put_handler_ = handler;
}

void HttpServer::setGetHandler(std::function<std::string()> handler) {
    get_handler_ = handler;
}

void HttpServer::start() {
    if (running_) return;
    
    running_ = true;
    server_thread_ = std::thread(&HttpServer::serverLoop, this);
}

void HttpServer::stop() {
    running_ = false;
    if (server_thread_.joinable()) {
        server_thread_.join();
    }
}

void HttpServer::serverLoop() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
#ifdef _WIN32
        closesocket(server_socket);
        WSACleanup();
#else
        close(server_socket);
#endif
        return;
    }

    if (listen(server_socket, 10) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
#ifdef _WIN32
        closesocket(server_socket);
        WSACleanup();
#else
        close(server_socket);
#endif
        return;
    }

    std::cout << "HTTP Server listening on port " << port_ << std::endl;

    while (running_) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
        
        if (client_socket >= 0) {
            std::thread(&HttpServer::handleClient, this, client_socket).detach();
        }
    }

#ifdef _WIN32
    closesocket(server_socket);
    WSACleanup();
#else
    close(server_socket);
#endif
}

void HttpServer::handleClient(int client_socket) {
    char buffer[4096];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::string request(buffer);
        
        std::string method, path, body;
        parseHttpRequest(request, method, path, body);
        
        std::string response;
        
        if (method == "GET" && path == "/items" && get_handler_) {
            std::string json_response = get_handler_();
            response = createHttpResponse(200, json_response);
        }
        else if (method == "POST" && path == "/items" && post_handler_) {
            std::string json_response = post_handler_(body);
            response = createHttpResponse(201, json_response);
        }
        else if (method == "PUT" && path.find("/items/") == 0 && put_handler_) {
            std::string id_str = path.substr(7); // Skip "/items/"
            size_t status_pos = id_str.find("/status");
            if (status_pos != std::string::npos) {
                id_str = id_str.substr(0, status_pos);
                int id = std::stoi(id_str);
                std::string json_response = put_handler_(id);
                response = createHttpResponse(200, json_response);
            } else {
                response = createHttpResponse(404, "{\"error\":\"Not found\"}");
            }
        }
        else {
            response = createHttpResponse(404, "{\"error\":\"Not found\"}");
        }
        
        send(client_socket, response.c_str(), response.length(), 0);
    }

#ifdef _WIN32
    closesocket(client_socket);
#else
    close(client_socket);
#endif
}

std::string HttpServer::parseHttpRequest(const std::string& request, std::string& method, std::string& path, std::string& body) {
    std::istringstream iss(request);
    std::string line;
    
    // Parse first line for method and path
    if (std::getline(iss, line)) {
        std::istringstream first_line(line);
        first_line >> method >> path;
    }
    
    // Skip headers and find body
    bool found_empty_line = false;
    while (std::getline(iss, line)) {
        if (line == "\r" || line.empty()) {
            found_empty_line = true;
            break;
        }
    }
    
    // Read body if present
    if (found_empty_line) {
        std::string remaining;
        while (std::getline(iss, line)) {
            body += line + "\n";
        }
        if (!body.empty() && body.back() == '\n') {
            body.pop_back();
        }
    }
    
    return "";
}

std::string HttpServer::createHttpResponse(int status_code, const std::string& body, const std::string& content_type) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " ";
    
    switch (status_code) {
        case 200: response << "OK"; break;
        case 201: response << "Created"; break;
        case 404: response << "Not Found"; break;
        default: response << "Unknown"; break;
    }
    
    response << "\r\n";
    response << "Content-Type: " << content_type << "\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "Access-Control-Allow-Origin: *\r\n";
    response << "Access-Control-Allow-Methods: GET, POST, PUT, OPTIONS\r\n";
    response << "Access-Control-Allow-Headers: Content-Type\r\n";
    response << "\r\n";
    response << body;
    
    return response.str();
}