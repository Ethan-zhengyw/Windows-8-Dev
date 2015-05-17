using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace App1
{
    public class MyColor
    {
        // 正确颜色名和RGB对应的查询字典 静态成员
        public static Dictionary<string, string> colorDict = new Dictionary<string,string>() {
            {"#FFFF0000", "红"}, {"#FF008000", "绿"}, {"#FF0000FF", "蓝"},
            {"#FFFFFF00", "黄"}, {"#FFFF1493", "粉"}, {"#FF800080", "紫"},
            {"#FFB8860B", "棕"}, {"#FF808080", "灰"}, {"#FFFFA500", "橙"}
        };
        
        // 使用静态成员colorDict进行判断
        public static bool match(string colorString, string colorText)
        {
            return (colorDict[colorString] == colorText);
        }

        public static List<List<string>> getCorrectColors()  // 每次返回的顺序都不一样
        {
            var tmp = new List<List<string>> {
                new List<string>{"Red", "红"}, new List<string>{"green", "绿"},
                new List<string>{"Blue", "蓝"}, new List<string>{"yellow", "黄"},
                new List<string>{"DeepPink", "粉"}, new List<string>{"Purple", "紫"},
                new List<string>{"DarkGoldenrod", "棕"}, new List<string>{"Gray", "灰"},
                new List<string>{"Orange", "橙"}
            };

            var correctColors = new List<List<string>>();

            for (int i = 9; i >= 1; i--) {
                int pos = new Random((int)DateTime.Now.Ticks).Next(0, i);
                correctColors.Add(tmp[pos]);
                tmp.RemoveAt(pos);
            }

            return correctColors;
        }

        public MyColor(string colorString_, string colorText_)
        {
            colorString = colorString_;
            colorText = colorText_;
        }

        public string ColorString { get { return colorString; } set { colorString = value; } }
        public string ColorText { get { return colorText; } set { colorText = value; } }
        public string ImgPath { get { return imgPath; } set { imgPath = value; } }

        private string colorString;  // 颜色的英文名称，如Red、Blue等，用于绑定设置颜色块的背景颜色或文本本身的颜色
        private string colorText;  // 颜色的中文名称，如红、蓝等
        private string imgPath;  // 这个属性是在自由模式中点击Pause后设置每个颜色块所显示内容图像的路径的
    }
}
