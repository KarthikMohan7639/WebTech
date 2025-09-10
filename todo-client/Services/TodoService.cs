using System.Net.Http;
using System.Text;
using System.Net.WebSockets;
using Newtonsoft.Json;
using TodoClient.Models;

namespace TodoClient.Services
{
    public class TodoService
    {
        private readonly HttpClient _httpClient;
        private ClientWebSocket? _webSocket;
        private readonly string _baseUrl = "http://localhost:8080";
        private readonly string _websocketUrl = "ws://localhost:8081";
        
        public event Action<TodoItem>? ItemAdded;
        public event Action<TodoItem>? ItemUpdated;

        public TodoService()
        {
            _httpClient = new HttpClient();
        }

        public async Task<List<TodoItem>> GetAllItemsAsync()
        {
            try
            {
                var response = await _httpClient.GetStringAsync($"{_baseUrl}/items");
                var items = JsonConvert.DeserializeObject<List<TodoItem>>(response);
                return items ?? new List<TodoItem>();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error getting items: {ex.Message}");
                return new List<TodoItem>();
            }
        }

        public async Task<TodoItem?> AddItemAsync(string description)
        {
            try
            {
                var requestData = new { description = description };
                var json = JsonConvert.SerializeObject(requestData);
                var content = new StringContent(json, Encoding.UTF8, "application/json");
                
                var response = await _httpClient.PostAsync($"{_baseUrl}/items", content);
                if (response.IsSuccessStatusCode)
                {
                    var responseJson = await response.Content.ReadAsStringAsync();
                    return JsonConvert.DeserializeObject<TodoItem>(responseJson);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error adding item: {ex.Message}");
            }
            return null;
        }

        public async Task<TodoItem?> UpdateItemStatusAsync(int id)
        {
            try
            {
                var response = await _httpClient.PutAsync($"{_baseUrl}/items/{id}/status", null);
                if (response.IsSuccessStatusCode)
                {
                    var responseJson = await response.Content.ReadAsStringAsync();
                    return JsonConvert.DeserializeObject<TodoItem>(responseJson);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error updating item status: {ex.Message}");
            }
            return null;
        }

        public async Task ConnectWebSocketAsync()
        {
            try
            {
                _webSocket = new ClientWebSocket();
                await _webSocket.ConnectAsync(new Uri(_websocketUrl), CancellationToken.None);
                
                // Start listening for messages
                _ = Task.Run(ListenForWebSocketMessages);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error connecting to WebSocket: {ex.Message}");
            }
        }

        private async Task ListenForWebSocketMessages()
        {
            if (_webSocket == null) return;

            var buffer = new byte[1024 * 4];
            
            try
            {
                while (_webSocket.State == WebSocketState.Open)
                {
                    var result = await _webSocket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);
                    
                    if (result.MessageType == WebSocketMessageType.Text)
                    {
                        var message = Encoding.UTF8.GetString(buffer, 0, result.Count);
                        await ProcessWebSocketMessage(message);
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"WebSocket error: {ex.Message}");
            }
        }

        private async Task ProcessWebSocketMessage(string message)
        {
            try
            {
                dynamic notification = JsonConvert.DeserializeObject(message)!;
                string type = notification.type;
                
                if (type == "item_added" && notification.item != null)
                {
                    var item = JsonConvert.DeserializeObject<TodoItem>(notification.item.ToString());
                    if (item != null)
                    {
                        await System.Windows.Application.Current.Dispatcher.InvokeAsync(() =>
                        {
                            ItemAdded?.Invoke(item);
                        });
                    }
                }
                else if (type == "item_updated" && notification.item != null)
                {
                    var item = JsonConvert.DeserializeObject<TodoItem>(notification.item.ToString());
                    if (item != null)
                    {
                        await System.Windows.Application.Current.Dispatcher.InvokeAsync(() =>
                        {
                            ItemUpdated?.Invoke(item);
                        });
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error processing WebSocket message: {ex.Message}");
            }
        }

        public void Dispose()
        {
            _webSocket?.Dispose();
            _httpClient?.Dispose();
        }
    }
}