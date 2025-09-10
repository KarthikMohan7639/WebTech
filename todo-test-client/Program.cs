using System;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace TodoTestClient
{
    class Program
    {
        private static readonly HttpClient client = new HttpClient();
        private static readonly string baseUrl = "http://localhost:8080";

        static async Task Main(string[] args)
        {
            Console.WriteLine("Todo List Test Client");
            Console.WriteLine("Make sure the C++ server is running on localhost:8080");
            Console.WriteLine();

            await TestApiEndpoints();
        }

        static async Task TestApiEndpoints()
        {
            try
            {
                // Test GET /items (should be empty initially)
                Console.WriteLine("1. Getting all items...");
                var items = await GetItems();
                Console.WriteLine($"Current items: {items}");
                Console.WriteLine();

                // Test POST /items
                Console.WriteLine("2. Adding new item...");
                var newItem = await AddItem("Learn C++ and C# integration");
                Console.WriteLine($"Added item: {newItem}");
                Console.WriteLine();

                // Add another item
                Console.WriteLine("3. Adding another item...");
                var newItem2 = await AddItem("Build todo application");
                Console.WriteLine($"Added item: {newItem2}");
                Console.WriteLine();

                // Test GET /items again
                Console.WriteLine("4. Getting all items after additions...");
                items = await GetItems();
                Console.WriteLine($"Current items: {items}");
                Console.WriteLine();

                // Test PUT /items/{id}/status
                Console.WriteLine("5. Updating status of first item...");
                var updatedItem = await UpdateItemStatus(1);
                Console.WriteLine($"Updated item: {updatedItem}");
                Console.WriteLine();

                // Test GET /items one more time
                Console.WriteLine("6. Final items list...");
                items = await GetItems();
                Console.WriteLine($"Final items: {items}");
                Console.WriteLine();

                Console.WriteLine("All tests completed successfully!");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
        }

        static async Task<string> GetItems()
        {
            var response = await client.GetStringAsync($"{baseUrl}/items");
            return response;
        }

        static async Task<string> AddItem(string description)
        {
            var requestData = new { description = description };
            var json = JsonConvert.SerializeObject(requestData);
            var content = new StringContent(json, Encoding.UTF8, "application/json");
            
            var response = await client.PostAsync($"{baseUrl}/items", content);
            return await response.Content.ReadAsStringAsync();
        }

        static async Task<string> UpdateItemStatus(int id)
        {
            var response = await client.PutAsync($"{baseUrl}/items/{id}/status", null);
            return await response.Content.ReadAsStringAsync();
        }
    }
}