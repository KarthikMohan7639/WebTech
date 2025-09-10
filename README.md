# Client-Server To-Do List Application

This repository contains a complete client-server application for managing a shared to-do list, implemented as specified in the requirements.

## Architecture

### C++ Server (`todo-server/`)
- **Communication**: HTTP REST API with WebSocket for real-time updates
- **Endpoints**:
  - `GET /items` - Retrieve all todo items
  - `POST /items` - Add a new item (JSON body: `{"description": "text"}`)
  - `PUT /items/{id}/status` - Toggle item status between Pending/Completed
- **Real-time**: WebSocket server on port 8081 broadcasts changes to all connected clients
- **Data**: In-memory storage with thread-safe operations

### C# WPF Client (`todo-client/`)
- **UI**: Clean WPF interface with ListView, checkboxes, and input controls
- **Communication**: HTTP client for API calls, WebSocket client for real-time updates
- **Features**: 
  - Real-time synchronization when other clients make changes
  - Add items with Enter key or button click
  - Toggle status with checkboxes
  - Visual strikethrough for completed items

## Building and Running

### C++ Server

**Prerequisites**: 
- CMake 3.10+
- C++17 compatible compiler
- Linux/Windows/macOS

**Build and Run**:
```bash
cd todo-server
mkdir build && cd build
cmake ..
make
./todo_server
```

The server will start on:
- HTTP API: http://localhost:8080
- WebSocket: ws://localhost:8081

### C# WPF Client

**Prerequisites**:
- .NET 6.0+ SDK
- Windows (WPF is Windows-only)

**Build and Run**:
```bash
cd todo-client
dotnet build
dotnet run
```

## Usage

1. Start the C++ server first
2. Run one or more C# client instances
3. Add items in any client - they appear in all clients immediately
4. Click checkboxes to toggle status - updates sync in real-time across all clients

## API Examples

```bash
# Get all items
curl http://localhost:8080/items

# Add new item
curl -X POST http://localhost:8080/items \
  -H "Content-Type: application/json" \
  -d '{"description":"Learn C++"}'

# Toggle item status
curl -X PUT http://localhost:8080/items/1/status
```

## Features Implemented

✅ **C++ Server Requirements**:
- Network communication for multiple clients
- Add Item endpoint with unique ID generation
- Update Status endpoint with ID-based toggling
- Get List endpoint returning all items
- Real-time synchronization via WebSocket
- In-memory data management with thread safety

✅ **C# WPF Client Requirements**:
- Clean WPF UI with ListView and input controls
- Startup connection and initial data fetch
- Add items and toggle status functionality
- Real-time UI updates without manual refresh
- Proper data binding and visual feedback