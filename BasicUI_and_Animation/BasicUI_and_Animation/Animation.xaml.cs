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

namespace BasicUI_and_Animation
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class Animation : Page
    {
        public Animation()
        {
            this.InitializeComponent();
        }

        private void backToPersonInfo(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(PersonInfo));
        }

        private void startAnimation(object sender, RoutedEventArgs e)
        {
            fishJump.Begin();
            boatComes.Begin();
        }

        private void stopAnimation(object sender, RoutedEventArgs e)
        {
            fishJump.Pause();
            boatComes.Pause();
        }

        private void fishSpeedChanged(object sender, RangeBaseValueChangedEventArgs e)
        {
            if (fishJump != null)
                fishJump.SpeedRatio = e.NewValue / 50;
        }
        private void boatSpeedChanged(object sender, RangeBaseValueChangedEventArgs e)
        {
            if (boatComes != null)
                boatComes.SpeedRatio = e.NewValue / 50;
        }
    }
}
