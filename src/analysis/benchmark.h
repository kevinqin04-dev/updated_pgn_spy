#include <vector>
#include <string>
using namespace std; 

namespace benchmark{
    vector<vector<vector<int>>> analyzeBenchmark(const string& filepath, int depth, bool white); 
    vector<int> analyzeOneGame(const vector<string>& game1, bool white, int depth, bp::opstream& in, bp::ipstream& out); 
}