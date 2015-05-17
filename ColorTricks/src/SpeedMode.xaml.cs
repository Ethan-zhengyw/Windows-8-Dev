using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI;
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
    public sealed partial class SpeedMode : Page
    {
        private List<MyColor> myColors;
        private MyColor correctColor;
        private List<MyColor> errorColors;
        private int currentScore;
        private int highestScore_speed;
        private int hardness;
        private int ranshif = 1;  // 通过偏移导致颜色和文本错位不匹配
        private int leftLife;
        private bool judge;
        private bool breakRecord;

        private DispatcherTimer timer = new DispatcherTimer(); // 用于倒计时
        private DispatcherTimer musicTimer = new DispatcherTimer(); // 用于倒计时
        private int leftTime; // 有60秒时间

        Windows.Storage.ApplicationDataContainer localsettings;

        public SpeedMode()
        {
            this.InitializeComponent();
            breakRecord = false;
            localsettings = Windows.Storage.ApplicationData.Current.LocalSettings;

            if (localsettings.Values["highestScore_speed"] == null) {
                highestScore_speed = 0;
                localsettings.Values["highestScore_speed"] = highestScore_speed;
            }
            highestScore_speed = int.Parse(localsettings.Values["highestScore_speed"].ToString());
            localsettings.Values["oldScore"] = highestScore_speed;
            history.Text = "Best:" + highestScore_speed.ToString();

            if (localsettings.Values["hardness"] == null) {
                hardness = 5;
                localsettings.Values["hardness"] = hardness;
            }
            hardness = int.Parse(localsettings.Values["hardness"].ToString());

            currentScore = 0;
            this.score.DataContext = currentScore;
            shulff();
            leftTime = hardness;
            leftLife = 1;
            startTimer();
            startTimerToPlayMusic();
        }
        private void startTimer()
        {
            timer.Tick += updateLeftTime;
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Start();
        }
        private void startTimerToPlayMusic()
        {
            musicTimer.Tick += playBackgroundMusic;
            musicTimer.Interval = TimeSpan.FromSeconds(1);
            musicTimer.Start();
        }

        private void playBackgroundMusic(object sender, object e)
        {
            TimeSpan start = (TimeSpan)localsettings.Values["musicPos"];
            backgroundMusic.Position = start;
            backgroundMusic.Play();
            musicTimer.Stop();
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

        public MyColor getCorrectColor()
        {
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

            /*for (int i = 0; i < 9; i++)
            {
                if (i == correctPos)
                {
                    myColors.Add(correctColor);
                }
                else
                {
                    if (i <= correctPos)
                    {
                        myColors.Add(errorColors[i]);
                    }
                    else
                    {
                        myColors.Add(errorColors[i - 1]);
                    }
                }
            }*/

            if (new Random().Next(0, 10) > 6) {  // 确定出现匹配情况概率
                myColors.Add(correctColor);
            } else {
                myColors.Add(errorColors[0]);
            }

            //targetColor.Text = myColors[0].ColorText;
            targetColor.DataContext = myColors[0];
            heartBeat.Begin();

            updateRanShift();
            leftTime = hardness;
            leftTimeBlock.Text = "Time:" + hardness.ToString();
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

            for (int i = 0; i < 9; i++)
            {
                if (tmp[i][0].Equals(correctColor.ColorString))
                {
                    tmp.RemoveAt(i);
                    break;
                }
            }

            for (int i = 0; i < 8; i++)
            {
                errorColors.Add(new MyColor(tmp[i][0], tmp[(i + ranshif) % 8][1]));
            }

            return errorColors;
        }

        private void zoomIn(object sender, PointerRoutedEventArgs e)
        {
            Image img = (Image)sender;
            img.Opacity = 1.0;
            makeBigger(sender);
        }

        private void makeBigger(object sender)
        {
            Image img = (Image)sender;
            Thickness margin = img.Margin;
            margin.Top -= 10;
            margin.Bottom -= 10;
            margin.Left -= 10;
            margin.Right -= 10;
            img.Margin = margin;
        }

        private void zoomOut(object sender, PointerRoutedEventArgs e)
        {
            Image img = (Image)sender;
            img.Opacity = 0.5;
            makeSmaller(sender);
        }

        private void makeSmaller(object sender)
        {
            Image img = (Image)sender;
            Thickness margin = img.Margin;
            margin.Top += 10;
            margin.Bottom += 10;
            margin.Left += 10;
            margin.Right += 10;
            img.Margin = margin;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            shulff();
        }

        private void exam()
        {
            clickSound.Play();
            if (!timer.IsEnabled)
                return;

            var colorString = ((SolidColorBrush)targetColor.Foreground).Color.ToString();
            var a = colorString;
            string colorText = targetColor.Text;

            if ((MyColor.match(colorString, colorText) && judge)
                || (!MyColor.match(colorString, colorText) && !judge)) {
                currentScore += 10;
                score.Text = currentScore.ToString();
                localsettings.Values["currentScore"] = currentScore;
                updateHistoryScore(currentScore);
            } else {
                giveWrongAnswerOrTimeUp();
            }
            shulff();
        }

        private void giveWrongAnswerOrTimeUp()
        {
            leftLife--;
            if (leftLife != 0)
            {
                leftTime = hardness;
            }
            else
            {
                localsettings.Values["mode"] = 1;  // 0-FreeMode, 1-SpeedMode
                localsettings.Values["currentScore"] = currentScore;
                timer.Stop();
                localsettings.Values["musicPos"] = backgroundMusic.Position;
                if (breakRecord)
                    this.Frame.Navigate(typeof(NewRecordPage));
                else
                    this.Frame.Navigate(typeof(GameOverPage));
            }
        }

        private void updateHistoryScore(int currentScore)
        {
            if (currentScore > highestScore_speed)
            {
                breakRecord = true;
                highestScore_speed = currentScore;
                Windows.Storage.ApplicationData.Current.LocalSettings.Values["highestScore_speed"] = highestScore_speed;
            }
        }

        private void toMainPage(object sender, RoutedEventArgs e)
        {
            timer.Stop();
            localsettings.Values["musicPos"] = backgroundMusic.Position;
            this.Frame.Navigate(typeof(MainPage));
        }

        private void pause(object sender, RoutedEventArgs e)
        {
            if (timer.IsEnabled)
            {
                timer.Stop();
                pauseButtonImg.Source = new BitmapImage(
                    new Uri(this.BaseUri, "Assets/FreeMode/Continue.png"));
            }
            else
            {
                shulff();
                timer.Start();
                pauseButtonImg.Source = new BitmapImage(
                    new Uri(this.BaseUri, "Assets/FreeMode/Pause.png"));
            }
        }

        private void makeSmall(object sender, RoutedEventArgs e)
        {
            makeSmaller(sender);
        }
        private void makeBig(object sender, RoutedEventArgs e)
        {
            makeBigger(sender);
            getJudge(sender);
            exam();
        }
        private void getJudge(object sender) {
            if (((Image)sender).Name == "check")
                judge = true;
            else
                judge = false;
        }
    }
}
