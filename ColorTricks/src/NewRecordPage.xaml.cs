using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel.DataTransfer;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage.Streams;
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
    public sealed partial class NewRecordPage : Page
    {
        Windows.Storage.ApplicationDataContainer localsettings;
        int mode;
        string currentScore, highestScore, highestScore_speed, oldScore;

        public NewRecordPage()
        {
            this.InitializeComponent();
            localsettings = Windows.Storage.ApplicationData.Current.LocalSettings;

            mode = int.Parse(localsettings.Values["mode"].ToString());
            currentScore = localsettings.Values["currentScore"].ToString();
            highestScore = localsettings.Values["highestScore"].ToString();
            highestScore_speed = localsettings.Values["highestScore_speed"].ToString();
            if (localsettings.Values["oldScore"] != null)
                oldScore = localsettings.Values["oldScore"].ToString();

            showScore();
        }

        private void showScore()
        {
            newRecordBlock.Text = "新纪录：" + currentScore;
            histroyScoreBlock.Text = ""; // "历史最高分：" + oldScore;
        }

        private void toMainPage(object sender, RoutedEventArgs e)
        {
            localsettings.Values["musicPos"] = backgroundMusic.Position;
            this.Frame.Navigate(typeof(MainPage));
        }

        private void toGamePage(object sender, RoutedEventArgs e)
        {
            localsettings.Values["musicPos"] = backgroundMusic.Position;
            if (mode == 0) {
                this.Frame.Navigate(typeof(FreeMode));
            } else {
                this.Frame.Navigate(typeof(SpeedMode));
            }
        }

        private void share(object sender, RoutedEventArgs e)
        {
            DataTransferManager.ShowShareUI();
        }
    }
}
