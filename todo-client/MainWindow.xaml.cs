using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using TodoClient.Models;
using TodoClient.Services;

namespace TodoClient
{
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        private readonly TodoService _todoService;
        
        public ObservableCollection<TodoItem> TodoItems { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            DataContext = this;
            
            TodoItems = new ObservableCollection<TodoItem>();
            _todoService = new TodoService();
            
            // Subscribe to real-time updates
            _todoService.ItemAdded += OnItemAdded;
            _todoService.ItemUpdated += OnItemUpdated;
            
            // Load initial data and connect WebSocket
            Loaded += MainWindow_Loaded;
        }

        private async void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            await LoadTodoItems();
            await _todoService.ConnectWebSocketAsync();
        }

        private async Task LoadTodoItems()
        {
            try
            {
                var items = await _todoService.GetAllItemsAsync();
                TodoItems.Clear();
                foreach (var item in items)
                {
                    TodoItems.Add(item);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading todo items: {ex.Message}", "Error", 
                    MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private async void AddButton_Click(object sender, RoutedEventArgs e)
        {
            await AddNewItem();
        }

        private async void NewItemTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                await AddNewItem();
            }
        }

        private async Task AddNewItem()
        {
            string description = NewItemTextBox.Text.Trim();
            if (string.IsNullOrEmpty(description))
            {
                MessageBox.Show("Please enter a description for the todo item.", "Validation Error",
                    MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            try
            {
                var newItem = await _todoService.AddItemAsync(description);
                if (newItem != null)
                {
                    NewItemTextBox.Text = string.Empty;
                    NewItemTextBox.Focus();
                }
                else
                {
                    MessageBox.Show("Failed to add the item. Please try again.", "Error",
                        MessageBoxButton.OK, MessageBoxImage.Error);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error adding item: {ex.Message}", "Error",
                    MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private async void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            await UpdateItemStatus(sender);
        }

        private async void CheckBox_Unchecked(object sender, RoutedEventArgs e)
        {
            await UpdateItemStatus(sender);
        }

        private async Task UpdateItemStatus(object sender)
        {
            if (sender is CheckBox checkBox && checkBox.DataContext is TodoItem item)
            {
                try
                {
                    var updatedItem = await _todoService.UpdateItemStatusAsync(item.Id);
                    if (updatedItem == null)
                    {
                        MessageBox.Show("Failed to update the item status. Please try again.", "Error",
                            MessageBoxButton.OK, MessageBoxImage.Error);
                        
                        // Revert the checkbox state
                        checkBox.IsChecked = !checkBox.IsChecked;
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Error updating item status: {ex.Message}", "Error",
                        MessageBoxButton.OK, MessageBoxImage.Error);
                    
                    // Revert the checkbox state
                    checkBox.IsChecked = !checkBox.IsChecked;
                }
            }
        }

        // Real-time update handlers
        private void OnItemAdded(TodoItem item)
        {
            // Check if item already exists (to avoid duplicates from our own actions)
            var existingItem = TodoItems.FirstOrDefault(x => x.Id == item.Id);
            if (existingItem == null)
            {
                TodoItems.Add(item);
            }
        }

        private void OnItemUpdated(TodoItem updatedItem)
        {
            var existingItem = TodoItems.FirstOrDefault(x => x.Id == updatedItem.Id);
            if (existingItem != null)
            {
                existingItem.Status = updatedItem.Status;
                existingItem.Description = updatedItem.Description;
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        protected override void OnClosed(EventArgs e)
        {
            _todoService.Dispose();
            base.OnClosed(e);
        }
    }

    // Converter for strikethrough text decoration
    public class BoolToTextDecorationsConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool isCompleted && isCompleted)
            {
                return TextDecorations.Strikethrough;
            }
            return null;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}