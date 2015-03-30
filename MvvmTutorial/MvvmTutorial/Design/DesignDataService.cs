using System.Threading.Tasks;
using MvvmTutorial.Model;
using System.Collections.Generic;

namespace MvvmTutorial.Design
{
    public class DesignDataService : IDataService
    {
        public Task<List<DataItem>> GetData()
        {
            // Use this to create design time data

            List<DataItem> books = new List<DataItem>() {
                new DataItem("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new DataItem("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new DataItem(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new DataItem("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new DataItem("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover"),
                new DataItem("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new DataItem("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new DataItem(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new DataItem("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new DataItem("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover"),
                new DataItem("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new DataItem("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new DataItem(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new DataItem("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new DataItem("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover"),
                new DataItem("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new DataItem("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new DataItem(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new DataItem("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new DataItem("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover"),
                new DataItem("程序员面试宝典", "欧力奇", "2011", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-121-12647-5/cover"),
                new DataItem("SQL应用开发参考手册", "郭鑫", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44169-4/cover"),
                new DataItem(".NET最佳实践", "里奇", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44912-6/cover"),
                new DataItem("Windows 8应用开发实战 : 使用JavaScript", "塞尔斯", "2014", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-111-44745-0/cover"),
                new DataItem("高效能程序员的修炼", "阿特伍德", "2013", "http://202.112.150.126/index.php?client=libcode&isbn=978-7-115-31898-5/cover")
            };

            return Task.FromResult(books);
        }
    }
}