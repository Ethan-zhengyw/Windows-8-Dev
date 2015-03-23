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
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// “空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=234238 上有介绍

namespace DataBinding_and_ViewTransition
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class GridView : Page
    {
        public GridView()
        {
            this.InitializeComponent();
            
            List<Book> books = new List<Book>() {
                new Book("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new Book("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new Book(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new Book("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new Book("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover"),
                new Book("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new Book("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new Book(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new Book("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new Book("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover"),
                new Book("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new Book("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new Book(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new Book("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new Book("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover"),
                new Book("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new Book("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new Book(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new Book("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new Book("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover"),
                new Book("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new Book("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new Book(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new Book("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new Book("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover")
            };

            bookGrid.ItemsSource = books;
            bookList.ItemsSource = books;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(MainPage));
            bookGrid.Visibility = Windows.UI.Xaml.Visibility.Visible;
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

        // 视图切换
        private void changeLayout(object sender, SizeChangedEventArgs e)
        {
            double width = e.NewSize.Width;
            double height  = e.NewSize.Height;

            if (width <= height)
            {
                bookGrid.Visibility = Visibility.Collapsed;
                bookList.Visibility = Visibility.Visible;
            }
            else
            {
                bookGrid.Visibility = Visibility.Visible;
                bookList.Visibility = Visibility.Collapsed;
            }
        }
    }
}
