#pragma once
#include <string>
#include <functional>
#include <thread>
#include <vector>
#include <memory>

class HttpServer {
private:
    int port_;
    bool running_;
    std::thread server_thread_;
    
    // Handler functions
    std::function<std::string(const std::string&)> post_handler_;
    std::function<std::string(int)> put_handler_;
    std::function<std::string()> get_handler_;
    
    void serverLoop();
    void handleClient(int client_socket);
    std::string parseHttpRequest(const std::string& request, std::string& method, std::string& path, std::string& body);
    std::string createHttpResponse(int status_code, const std::string& body, const std::string& content_type = "application/json");

public:
    HttpServer(int port);
    ~HttpServer();
    
    void setPostHandler(std::function<std::string(const std::string&)> handler);
    void setPutHandler(std::function<std::string(int)> handler);
    void setGetHandler(std::function<std::string()> handler);
    
    void start();
    void stop();
};