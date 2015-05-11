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
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Navigation;

// “空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=234238 上有介绍

namespace App1
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class FreeMode : Page
    {
        private List<MyColor> myColors;
        private MyColor correctColor;
        private List<MyColor> errorColors;
        private int currentScore;
        private int highestScore;
        private int ranshif = 1;  // 通过偏移导致颜色和文本错位不匹配

        private DispatcherTimer timer = new DispatcherTimer(); // 用于倒计时
        private int leftTime; // 有60秒时间

        Windows.Storage.ApplicationDataContainer localsettings;

        public FreeMode()
        {
            this.InitializeComponent();
            localsettings = Windows.Storage.ApplicationData.Current.LocalSettings;

            if (localsettings.Values["highestScore"] == null) {
                highestScore = 0;
                localsettings.Values["highestScore"] = highestScore;
            }
            highestScore = int.Parse(localsettings.Values["highestScore"].ToString());
            history.Text = "Best:" + highestScore.ToString();
            currentScore = 0;
            this.score.DataContext = currentScore;
            shulff();
            leftTime = 60;
            startTimer();
        }
        private void startTimer()
        {
            timer.Tick += updateLeftTime;
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Start();
        }

        private void updateLeftTime(object sender, object e)
        {
            leftTimeBlock.Text = "Time:" + leftTime.ToString();
            if (leftTime != 0) {
                leftTime--;
            } else {
                giveWrongAnswerOrTimeUp();
            }
        }
        
        public MyColor getCorrectColor() {
            // 打乱顺序，为了使得正确的地方出现的颜色和文本不同
            List<List<string>> correctColors = MyColor.getCorrectColors();
            var correctColor = correctColors[new Random((int)DateTime.Now.Ticks).Next(0, 9)];
            return new MyColor(correctColor[0], correctColor[1]);
        }

        public void shulff()
        {
            int correctPos = new Random().Next(0, 9);
            myColors = new List<MyColor>();
            correctColor = getCorrectColor();
            errorColors = getErrorColors();
            

            for (int i = 0; i < 9; i++)
            {
                if (i == correctPos) {
                    myColors.Add(correctColor);
                } else {
                    if (i <= correctPos) {
                        myColors.Add(errorColors[i]);
                    } else {
                        myColors.Add(errorColors[i - 1]);
                    }
                }
            }
            colorGrid.ItemsSource = myColors;
            updateRanShift();
        }

        private void updateRanShift()
        {
            ranshif = (ranshif + 1) % 9;
            if (ranshif == 0)
                ranshif = 1;
        }

        private List<MyColor> getErrorColors()
        {
            List<MyColor> errorColors = new List<MyColor>();
            List<List<string>> tmp = MyColor.getCorrectColors();

            for (int i = 0; i < 9; i++) {
                if (tmp[i][0].Equals(correctColor.ColorString)) {
                    tmp.RemoveAt(i);
                    break;
                }
            }

            for (int i = 0; i < 8; i++) {
                errorColors.Add(new MyColor(tmp[i][0], tmp[(i + ranshif) % 8][1]));
            }

            return errorColors;
        }

        private void zoomIn(object sender, PointerRoutedEventArgs e)
        {
            Grid grid = (Grid)sender;
            ((TextBlock)grid.Children[0]).FontSize += 30;
        }

        private void zoomOut(object sender, PointerRoutedEventArgs e)
        {
            Grid grid = (Grid)sender;
            ((TextBlock)grid.Children[0]).FontSize -= 30;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            shulff();
        }

        private void exam(object sender, PointerRoutedEventArgs e)
        {
            if (!timer.IsEnabled)
                return;

            Grid grid = (Grid)sender;
            var colorString = ((SolidColorBrush)grid.Background).Color.ToString();
            var a = colorString;
            string colorText = ((TextBlock)grid.Children[0]).Text;

            if (MyColor.match(colorString, colorText)) {
                currentScore += 10;
                score.Text = currentScore.ToString();
                updateHistoryScore(currentScore);
            } else {
                updateHistoryScore(currentScore);
                giveWrongAnswerOrTimeUp();
            }
            shulff();
        }

        private void giveWrongAnswerOrTimeUp()
        {
            localsettings.Values["mode"] = 0;  // 0-FreeMode, 1-SpeedMode
            localsettings.Values["currentScore"] = currentScore;
            timer.Stop();
            this.Frame.Navigate(typeof(GameOverPage));
        }

        private void updateHistoryScore(int currentScore)
        {
            if (currentScore > highestScore)
            {
                highestScore = currentScore;
                Windows.Storage.ApplicationData.Current.LocalSettings.Values["highestScore"] = highestScore;
            }
        }

        private void toMainPage(object sender, RoutedEventArgs e)
        {
            timer.Stop();
            this.Frame.Navigate(typeof(MainPage));
        }

        private void pause(object sender, RoutedEventArgs e)
        {
            if (timer.IsEnabled) {
                timer.Stop();
                pauseButtonImg.Source = new BitmapImage(
                    new Uri(this.BaseUri, "Assets/FreeMode/Continue.png"));
                hide();
            } else {
                shulff();
                timer.Start();
                pauseButtonImg.Source = new BitmapImage(
                    new Uri(this.BaseUri, "Assets/FreeMode/Pause.png"));
            }
        }

        public void hide()
        {
            myColors = new List<MyColor>();

            for (int i = 0; i < 9; i++)
            {
                myColors.Add(new MyColor("#FFE0A500", ""));
                myColors[i].ImgPath = "Assets/Rabbits/Card" + (i + 1).ToString() + ".png";
            }
            colorGrid.ItemsSource = myColors;
            updateRanShift();
        }
    }
}
