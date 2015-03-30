using System.Threading.Tasks;
using System.Collections.Generic;

namespace MvvmTutorial.Model
{
    public interface IDataService
    {
        Task<List<DataItem>> GetData();
    }
}