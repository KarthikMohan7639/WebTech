#pragma once
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <functional>

class WebSocketServer {
private:
    int port_;
    bool running_;
    std::thread server_thread_;
    std::vector<int> client_sockets_;
    std::mutex clients_mutex_;
    
    void serverLoop();
    void handleClient(int client_socket);
    bool performWebSocketHandshake(int client_socket);
    void sendWebSocketMessage(int client_socket, const std::string& message);
    std::string generateWebSocketAccept(const std::string& key);

public:
    WebSocketServer(int port);
    ~WebSocketServer();
    
    void start();
    void stop();
    void broadcastMessage(const std::string& message);
};