#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <memory>

struct TodoItem {
    int id;
    std::string description;
    std::string status; // "Pending" or "Completed"
    
    TodoItem(int id, const std::string& desc) 
        : id(id), description(desc), status("Pending") {}
};

class TodoManager {
private:
    std::vector<TodoItem> items;
    std::mutex mutex_;
    int next_id_;

public:
    TodoManager();
    
    // Add new item and return the created item
    TodoItem addItem(const std::string& description);
    
    // Update item status by ID
    bool updateStatus(int id);
    
    // Get all items
    std::vector<TodoItem> getAllItems();
    
    // Convert item to JSON string
    std::string itemToJson(const TodoItem& item);
    
    // Convert all items to JSON array
    std::string allItemsToJson();
};