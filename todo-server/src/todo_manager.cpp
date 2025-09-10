#include "todo_manager.h"
#include <sstream>
#include <algorithm>

TodoManager::TodoManager() : next_id_(1) {}

TodoItem TodoManager::addItem(const std::string& description) {
    std::lock_guard<std::mutex> lock(mutex_);
    TodoItem item(next_id_++, description);
    items.push_back(item);
    return item;
}

bool TodoManager::updateStatus(int id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find_if(items.begin(), items.end(), 
                          [id](const TodoItem& item) { return item.id == id; });
    
    if (it != items.end()) {
        it->status = (it->status == "Pending") ? "Completed" : "Pending";
        return true;
    }
    return false;
}

std::vector<TodoItem> TodoManager::getAllItems() {
    std::lock_guard<std::mutex> lock(mutex_);
    return items;
}

std::string TodoManager::itemToJson(const TodoItem& item) {
    std::ostringstream json;
    json << "{\"id\":" << item.id 
         << ",\"description\":\"" << item.description 
         << "\",\"status\":\"" << item.status << "\"}";
    return json.str();
}

std::string TodoManager::allItemsToJson() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < items.size(); ++i) {
        if (i > 0) json << ",";
        json << itemToJson(items[i]);
    }
    json << "]";
    return json.str();
}