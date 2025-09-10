#include "http_server.h"
#include "websocket_server.h"
#include "todo_manager.h"
#include <iostream>
#include <signal.h>
#include <atomic>
#include <sstream>

std::atomic<bool> running(true);
TodoManager* todo_manager = nullptr;
WebSocketServer* ws_server = nullptr;

void signalHandler(int signal) {
    std::cout << "\nShutting down server..." << std::endl;
    running = false;
}

std::string parseJsonDescription(const std::string& json) {
    // Simple JSON parsing for {"description":"text"}
    size_t desc_pos = json.find("\"description\":");
    if (desc_pos == std::string::npos) return "";
    
    desc_pos = json.find("\"", desc_pos + 14); // Skip to opening quote
    if (desc_pos == std::string::npos) return "";
    
    size_t end_pos = json.find("\"", desc_pos + 1);
    if (end_pos == std::string::npos) return "";
    
    return json.substr(desc_pos + 1, end_pos - desc_pos - 1);
}

int main() {
    std::cout << "Starting Todo Server..." << std::endl;
    
    // Setup signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    // Initialize components
    todo_manager = new TodoManager();
    HttpServer http_server(8080);
    ws_server = new WebSocketServer(8081);
    
    // Setup HTTP handlers
    http_server.setGetHandler([&]() -> std::string {
        std::cout << "GET /items" << std::endl;
        return todo_manager->allItemsToJson();
    });
    
    http_server.setPostHandler([&](const std::string& body) -> std::string {
        std::cout << "POST /items: " << body << std::endl;
        std::string description = parseJsonDescription(body);
        if (description.empty()) {
            return "{\"error\":\"Invalid request\"}";
        }
        
        TodoItem item = todo_manager->addItem(description);
        std::string json_response = todo_manager->itemToJson(item);
        
        // Notify all WebSocket clients
        std::string notification = "{\"type\":\"item_added\",\"item\":" + json_response + "}";
        ws_server->broadcastMessage(notification);
        
        return json_response;
    });
    
    http_server.setPutHandler([&](int id) -> std::string {
        std::cout << "PUT /items/" << id << "/status" << std::endl;
        bool success = todo_manager->updateStatus(id);
        if (!success) {
            return "{\"error\":\"Item not found\"}";
        }
        
        // Find the updated item and return it
        auto items = todo_manager->getAllItems();
        for (const auto& item : items) {
            if (item.id == id) {
                std::string json_response = todo_manager->itemToJson(item);
                
                // Notify all WebSocket clients
                std::string notification = "{\"type\":\"item_updated\",\"item\":" + json_response + "}";
                ws_server->broadcastMessage(notification);
                
                return json_response;
            }
        }
        
        return "{\"error\":\"Item not found\"}";
    });
    
    // Start servers
    http_server.start();
    ws_server->start();
    
    std::cout << "Todo Server is running!" << std::endl;
    std::cout << "HTTP API: http://localhost:8080" << std::endl;
    std::cout << "WebSocket: ws://localhost:8081" << std::endl;
    std::cout << "Press Ctrl+C to stop the server." << std::endl;
    
    // Keep main thread alive
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Cleanup
    http_server.stop();
    ws_server->stop();
    delete ws_server;
    delete todo_manager;
    
    std::cout << "Server stopped." << std::endl;
    return 0;
}