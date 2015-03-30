using System;
using System.ComponentModel;

namespace MvvmTutorial.Model
{
    public class DataItem : INotifyPropertyChanged
    {

        public string Title { get { return title; } private set { title = value; NotifyPropertyChanged("Title"); } }
        public string Author { get { return author; } private set { author = value; NotifyPropertyChanged("Author"); } }
        public string Year { get { return year; } private set { year = value; NotifyPropertyChanged("Year"); } }
        public Uri CoverUri { get { return coverUri; } private set { coverUri = value; NotifyPropertyChanged("CoverUri"); } }

        private string title;
        private string author;
        private string year;
        private Uri coverUri;

        public DataItem(string title_, string author_, string year_, string coverUri_)
        {
            title = title_;
            author = author_;
            year = year_;
            coverUri = new Uri(coverUri_, UriKind.Absolute);
        }

        #region 属性变化事件
        public event PropertyChangedEventHandler PropertyChanged;

        private void NotifyPropertyChanged(string SomeString)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(SomeString));
            }
        }
        #endregion
    }
}