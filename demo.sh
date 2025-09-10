#!/bin/bash

echo "=== Todo List Application Demo ==="
echo
echo "This script demonstrates the client-server todo application."
echo "It will start the C++ server and run the test client."
echo
echo "Press Enter to continue..."
read

echo "1. Building the C++ server..."
cd todo-server
mkdir -p build && cd build
cmake .. && make
if [ $? -ne 0 ]; then
    echo "Error: Failed to build C++ server"
    exit 1
fi
cd ../..

echo
echo "2. Building the C# test client..."
cd todo-test-client
dotnet build
if [ $? -ne 0 ]; then
    echo "Error: Failed to build C# test client"
    exit 1
fi
cd ..

echo
echo "3. Starting the C++ server..."
echo "   HTTP API: http://localhost:8080"
echo "   WebSocket: ws://localhost:8081"
echo
(cd todo-server/build && ./todo_server) &
SERVER_PID=$!

# Wait for server to start
sleep 3

echo "4. Running the test client..."
echo
cd todo-test-client
dotnet run

echo
echo "5. Testing manual API calls..."
echo
echo "Adding items via curl:"
curl -s -X POST http://localhost:8080/items \
  -H "Content-Type: application/json" \
  -d '{"description":"Task from curl"}' | jq
echo

echo "Getting all items:"
curl -s http://localhost:8080/items | jq
echo

echo "Toggling status of item 3:"
curl -s -X PUT http://localhost:8080/items/3/status | jq
echo

echo "Final items list:"
curl -s http://localhost:8080/items | jq
echo

echo "Demo completed! Stopping server..."
kill $SERVER_PID
echo "Server stopped."