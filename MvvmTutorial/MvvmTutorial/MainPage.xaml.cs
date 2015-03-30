using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using MvvmTutorial.ViewModel;
using System.ComponentModel;
using MvvmTutorial.Common;
using Windows.ApplicationModel.Search;
using Windows.UI.Xaml.Navigation;
using MvvmTutorial.Model;

namespace MvvmTutorial
{
    public sealed partial class MainPage
    {
        private NavigationHelper navi;
        private MainViewModel vmodel;
        /// <summary>
        /// Gets the view's ViewModel.
        /// </summary>
        public MainViewModel Vm
        {
            get
            {
                return (MainViewModel)DataContext;
            }
        }

        public MainPage()
        {
            InitializeComponent();
            this.navi = new NavigationHelper(this);
            this.navi.LoadState += navi_LoadState;
            this.navi.SaveState += navi_SaveState;
            SearchPane.GetForCurrentView().SuggestionsRequested += searchPane_SuggestionsRequested;
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            //由App.xaml.cs导航过来后提供数据
            navi.OnNavigatedTo(e);
            vmodel = (MainViewModel)e.Parameter;

           /* vmodel.BookList.Add(new DataItem("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"));
            vmodel.BookList.Add(new DataItem("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"));
            vmodel.BookList.Add(new DataItem(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"));
            vmodel.BookList.Add(new DataItem("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"));
            vmodel.BookList.Add(new DataItem("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover"));
            *///推送数据到UI
            bookGrid.ItemsSource = vmodel.BookList;

            vmodel.PropertyChanged += (sender, e1) =>
            {
                if (e1.PropertyName == "SelectedItemIndex")
                {
                    bookGrid.SelectedIndex = vmodel.SelectedItemIndex;
                }
            };
        }

        void searchPane_SuggestionsRequested(SearchPane sender, SearchPaneSuggestionsRequestedEventArgs args)
        {
            foreach (Model.DataItem temp in vmodel.BookList)
            {
                string MaybeRight = temp.Title;

                if (MaybeRight.StartsWith(args.QueryText, StringComparison.CurrentCultureIgnoreCase))
                {
                    args.Request.SearchSuggestionCollection.AppendQuerySuggestion(MaybeRight);
                }
                if (args.Request.SearchSuggestionCollection.Size > 5)
                {
                    break;
                }
            }
        }
        private void navi_LoadState(object sender, LoadStateEventArgs e)
        {
        }
        private void navi_SaveState(object sender, SaveStateEventArgs e)
        {
        }
        protected override void LoadState(object state)
        {
            var casted = state as MainPageState;

            if (casted != null)
            {
                Vm.Load(casted.LastVisit);
            }
        }

        protected override object SaveState()
        {
            return new MainPageState
            {
                LastVisit = DateTime.Now
            };
        }

        private void showDetails(object sender, PointerRoutedEventArgs e)
        {
            UIElementCollection textBlocks = ((Grid)sender).Children;
            textBlocks[1].Visibility = Windows.UI.Xaml.Visibility.Visible;
        }

        private void hidDetails(object sender, PointerRoutedEventArgs e)
        {
            UIElementCollection textBlocks = ((Grid)sender).Children;
            textBlocks[1].Visibility = Windows.UI.Xaml.Visibility.Collapsed;
        }

    }

    public class MainPageState
    {
        public DateTime LastVisit
        {
            get;
            set;
        }
    }
}