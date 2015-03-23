using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataBinding_and_ViewTransition
{
    public class Book
    {
        public Book(string title_, string author_, string year_, string coverUri_)
        {
            title = title_;
            author = author_;
            year = year_;
            coverUri = new Uri(coverUri_, UriKind.Absolute);
        }

        public string Title { get { return title; } set { title = value; } }
        public string Author { get { return author; } set { author = value; } }
        public string Year { get { return year; } set { year = value; } }
        public Uri CoverUri { get { return coverUri; } set { coverUri = value; } }

        private string title;
        private string author;
        private string year;
        private Uri coverUri;
    }
}
