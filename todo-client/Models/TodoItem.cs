using System.ComponentModel;

namespace TodoClient.Models
{
    public class TodoItem : INotifyPropertyChanged
    {
        private string _status = "Pending";

        public int Id { get; set; }
        public string Description { get; set; } = string.Empty;
        
        public string Status
        {
            get => _status;
            set
            {
                if (_status != value)
                {
                    _status = value;
                    OnPropertyChanged(nameof(Status));
                    OnPropertyChanged(nameof(IsCompleted));
                }
            }
        }

        public bool IsCompleted
        {
            get => Status == "Completed";
            set
            {
                Status = value ? "Completed" : "Pending";
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}