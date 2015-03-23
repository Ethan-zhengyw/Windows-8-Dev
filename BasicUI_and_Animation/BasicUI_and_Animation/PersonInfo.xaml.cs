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

using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.UI.Xaml.Media.Imaging;

// “空白页”项模板在 http://go.microsoft.com/fwlink/?LinkId=234238 上有介绍

namespace BasicUI_and_Animation
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class PersonInfo : Page
    {
        public PersonInfo()
        {
            this.InitializeComponent();
        }

        /**
         * the Icon button was clicked and this function 
         * will be called to select another picther to replace the current one
         */
        private async void selectIcon(object sender, RoutedEventArgs e)
        {
            FileOpenPicker imagePicker = new FileOpenPicker();
            //imagePicker.SuggestedStartLocation = PickerLocationId.PicturesLibrary;  // 默认起始目录为图片收藏
            imagePicker.SuggestedStartLocation = PickerLocationId.PicturesLibrary;
            imagePicker.ViewMode = PickerViewMode.List;

            // 设置查找的文件类型，均为图片
            imagePicker.FileTypeFilter.Add(".bmp");
            imagePicker.FileTypeFilter.Add(".png");
            imagePicker.FileTypeFilter.Add(".jpg");
            imagePicker.FileTypeFilter.Add(".jpeg");

            //打开单个图片
            StorageFile file = await imagePicker.PickSingleFileAsync();
            if (file != null)
            {
                var stream = await file.OpenAsync(FileAccessMode.Read);
                var image = new BitmapImage();
                image.SetSource(stream);
                this.Icon.Source = image;
            }
        }

        private void viewAnimation(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(Animation));
        }
        private void backToTestPage(object sender, RoutedEventArgs e)
        {
            this.Frame.Navigate(typeof(MainPage));
        }
    }
}
