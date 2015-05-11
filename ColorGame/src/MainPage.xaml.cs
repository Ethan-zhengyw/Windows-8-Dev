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

namespace App1
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private void toFreeMode(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(FreeMode));
        }

        private void toSpeedMode(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(SpeedMode));
        }

        private void zoomIn(object sender, PointerRoutedEventArgs e)
        {
            Image img = (Image)sender;
            Thickness margin = img.Margin;
            margin.Top -= 10;
            margin.Bottom -= 10;
            img.Margin = margin;
            img.Opacity = 1.0;
        }

        private void zoomOut(object sender, PointerRoutedEventArgs e)
        {
            Image img = (Image)sender;
            Thickness margin = img.Margin;
            margin.Top += 10;
            margin.Bottom += 10;
            img.Margin = margin;
            img.Opacity = 0.8;
        }

        private void toHelpPage(object sender, PointerRoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(HelpPage));
        }

        private void toSettingPage(object sender, PointerRoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(SettingPage));
        }
    }
}
