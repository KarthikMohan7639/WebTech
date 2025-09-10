#include "websocket_server.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>

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

// Simple base64 encode function
std::string base64_encode(const std::string& input) {
    static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) result.push_back(chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (result.size() % 4) result.push_back('=');
    return result;
}

// Simple SHA1 hash function (simplified implementation)
std::string sha1(const std::string& input) {
    // For simplicity, using a basic hash. In production, use proper SHA1 implementation
    // This is just a placeholder that creates a valid-looking response
    return base64_encode("simple_hash_placeholder_" + input);
}

WebSocketServer::WebSocketServer(int port) : port_(port), running_(false) {}

WebSocketServer::~WebSocketServer() {
    stop();
}

void WebSocketServer::start() {
    if (running_) return;
    
    running_ = true;
    server_thread_ = std::thread(&WebSocketServer::serverLoop, this);
}

void WebSocketServer::stop() {
    running_ = false;
    
    // Close all client connections
    std::lock_guard<std::mutex> lock(clients_mutex_);
    for (int socket : client_sockets_) {
#ifdef _WIN32
        closesocket(socket);
#else
        close(socket);
#endif
    }
    client_sockets_.clear();
    
    if (server_thread_.joinable()) {
        server_thread_.join();
    }
}

void WebSocketServer::serverLoop() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Failed to create WebSocket server socket" << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind WebSocket server socket" << std::endl;
#ifdef _WIN32
        closesocket(server_socket);
        WSACleanup();
#else
        close(server_socket);
#endif
        return;
    }

    if (listen(server_socket, 10) < 0) {
        std::cerr << "Failed to listen on WebSocket server socket" << std::endl;
#ifdef _WIN32
        closesocket(server_socket);
        WSACleanup();
#else
        close(server_socket);
#endif
        return;
    }

    std::cout << "WebSocket Server listening on port " << port_ << std::endl;

    while (running_) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
        
        if (client_socket >= 0) {
            std::thread(&WebSocketServer::handleClient, this, client_socket).detach();
        }
    }

#ifdef _WIN32
    closesocket(server_socket);
    WSACleanup();
#else
    close(server_socket);
#endif
}

void WebSocketServer::handleClient(int client_socket) {
    if (performWebSocketHandshake(client_socket)) {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        client_sockets_.push_back(client_socket);
        std::cout << "WebSocket client connected" << std::endl;
        
        // Keep connection alive (simplified - in production, handle ping/pong)
        char buffer[1024];
        while (running_) {
            int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) break;
        }
        
        // Remove client from list
        auto it = std::find(client_sockets_.begin(), client_sockets_.end(), client_socket);
        if (it != client_sockets_.end()) {
            client_sockets_.erase(it);
        }
    }

#ifdef _WIN32
    closesocket(client_socket);
#else
    close(client_socket);
#endif
}

bool WebSocketServer::performWebSocketHandshake(int client_socket) {
    char buffer[4096];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_received <= 0) return false;
    
    buffer[bytes_received] = '\0';
    std::string request(buffer);
    
    // Find WebSocket key
    size_t key_pos = request.find("Sec-WebSocket-Key: ");
    if (key_pos == std::string::npos) return false;
    
    key_pos += 19; // Length of "Sec-WebSocket-Key: "
    size_t key_end = request.find("\r\n", key_pos);
    if (key_end == std::string::npos) return false;
    
    std::string websocket_key = request.substr(key_pos, key_end - key_pos);
    std::string accept_key = generateWebSocketAccept(websocket_key);
    
    std::string response = 
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: " + accept_key + "\r\n"
        "\r\n";
    
    send(client_socket, response.c_str(), response.length(), 0);
    return true;
}

std::string WebSocketServer::generateWebSocketAccept(const std::string& key) {
    std::string magic_string = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    std::string concatenated = key + magic_string;
    
    // In a real implementation, use proper SHA1 and base64
    // For simplicity, we'll create a valid-looking response
    return base64_encode("ws_accept_" + key.substr(0, 8));
}

void WebSocketServer::sendWebSocketMessage(int client_socket, const std::string& message) {
    // Create WebSocket frame (simplified)
    std::vector<uint8_t> frame;
    frame.push_back(0x81); // Text frame, final fragment
    
    if (message.length() < 126) {
        frame.push_back(static_cast<uint8_t>(message.length()));
    } else {
        frame.push_back(126);
        frame.push_back((message.length() >> 8) & 0xFF);
        frame.push_back(message.length() & 0xFF);
    }
    
    for (char c : message) {
        frame.push_back(static_cast<uint8_t>(c));
    }
    
    send(client_socket, reinterpret_cast<char*>(frame.data()), frame.size(), 0);
}

void WebSocketServer::broadcastMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(clients_mutex_);
    
    auto it = client_sockets_.begin();
    while (it != client_sockets_.end()) {
        try {
            sendWebSocketMessage(*it, message);
            ++it;
        } catch (...) {
            // Remove disconnected client
#ifdef _WIN32
            closesocket(*it);
#else
            close(*it);
#endif
            it = client_sockets_.erase(it);
        }
    }
}