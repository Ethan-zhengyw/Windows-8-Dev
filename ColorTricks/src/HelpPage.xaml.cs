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
    public sealed partial class HelpPage : Page
    {
        Windows.Storage.ApplicationDataContainer localsettings;
        private DispatcherTimer musicTimer = new DispatcherTimer(); // 用于倒计时
        public HelpPage()
        {
            this.InitializeComponent();
            localsettings = Windows.Storage.ApplicationData.Current.LocalSettings;
            startTimerToPlayMusic();
        }
        private void startTimerToPlayMusic()
        {
            musicTimer.Tick += playBackgroundMusic;
            musicTimer.Interval = TimeSpan.FromSeconds(0.5);
            musicTimer.Start();
        }
        private void playBackgroundMusic(object sender, object e)
        {
            TimeSpan start = (TimeSpan)localsettings.Values["musicPos"];
            backgroundMusic.Position = start;
            backgroundMusic.Play();
            musicTimer.Stop();
        }


        private void toMainPage(object sender, RoutedEventArgs e)
        {
            localsettings.Values["musicPos"] = backgroundMusic.Position;
            this.Frame.Navigate(typeof(MainPage));
        }
        
    }
}
