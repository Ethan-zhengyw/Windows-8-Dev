using System;
using System.Threading.Tasks;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using GalaSoft.MvvmLight.Views;
using MvvmTutorial.Common;
using MvvmTutorial.Model;
using System.Collections.Generic;
using System.ComponentModel;

namespace MvvmTutorial.ViewModel
{
    /// <summary>
    /// This class contains properties that the main View can data bind to.
    /// <para>
    /// See http://www.galasoft.ch/mvvm
    /// </para>
    /// </summary>
    public class MainViewModel : ViewModelBase, INotifyPropertyChanged
    {
        /// <summary>
        /// The <see cref="WelcomeTitle" /> property's name.
        /// </summary>
        public const string WelcomeTitlePropertyName = "WelcomeTitle";

        private readonly IDataService _dataService;
        private readonly INavigationService _navigationService;

        private RelayCommand _navigateCommand;
        private string _originalTitle;
        private string _welcomeTitle = string.Empty;
        private List<DataItem> _bookList;
        private int selectedItemIndex;
        public int SelectedItemIndex
        {
            get
            {
                return selectedItemIndex;
            }
            set
            {
                selectedItemIndex = value;
                NotifyPropertyChanged("SelectedItemIndex");
            }
        }
        //注册事件 PropertyChangedEventHandler
        public event PropertyChangedEventHandler PropertyChanged;
        public void NotifyPropertyChanged(string str)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(str));
            }
        }
        public void Search(string searchTerm)
        {
            int selectedItemIndex = -1;
            for (int i = 0; i < _bookList.Count; i++)
            {
                if (_bookList[i].Title.ToLower().Contains(searchTerm.ToLower()))
                {
                    selectedItemIndex = i;
                }
            }
            SelectedItemIndex = selectedItemIndex;
        }
        /// <summary>
        /// Gets the NavigateCommand.
        /// </summary>
        public RelayCommand NavigateCommand
        {
            get
            {
                return _navigateCommand
                       ?? (_navigateCommand = new RelayCommand(
                           () => _navigationService.NavigateTo(ViewModelLocator.SecondPageKey)));
            }
        }

        /// <summary>
        /// Gets the WelcomeTitle property.
        /// Changes to that property's value raise the PropertyChanged event. 
        /// </summary>
        public string WelcomeTitle
        {
            get
            {
                return _welcomeTitle;
            }

            set
            {
                Set(ref _welcomeTitle, value);
            }
        }

        public List<DataItem> BookList
        {
            get
            {
                return _bookList;
            }
            set
            {
                Set(ref _bookList, value);
            }
        }

        /// <summary>
        /// Initializes a new instance of the MainViewModel class.
        /// </summary>
        public MainViewModel()
        {
            _dataService = new DataService();
            _navigationService = new NavigationService();
            _bookList = new List<DataItem>();
            selectedItemIndex = -1;
            Initialize();
        }

        ////public override void Cleanup()
        ////{
        ////    // Clean up if needed

        ////    base.Cleanup();
        ////}
        public void Load(DateTime lastVisit)
        {
            if (lastVisit > DateTime.MinValue)
            {
                WelcomeTitle = string.Format(
                    "{0} (last visit on the {1})",
                    _originalTitle,
                    lastVisit);
                BookList = _bookList;
            }
        }

        private async Task Initialize()
        {
            try
            {
                var item = await _dataService.GetData();
                BookList = item;
                _originalTitle = "图书列表";
                WelcomeTitle = "图书列表";
            }
            catch (Exception ex)
            {
                // Report error here
            }
        }
    }
}