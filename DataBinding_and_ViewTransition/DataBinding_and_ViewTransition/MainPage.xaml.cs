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
using Windows.UI.Xaml.Media.Imaging;
// “空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=234238 上有介绍

namespace DataBinding_and_ViewTransition
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {


        public MainPage()
        {
            this.InitializeComponent();

            List<Book> books = new List<Book>() {
                new Book("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new Book("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new Book(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new Book("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new Book("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover")
            };

            bookFliper.ItemsSource = books;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(GridView));
        }
    }
}
