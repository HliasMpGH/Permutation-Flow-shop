#include <map>
#include <string>
#include <vector>

#include "Solution.h"

class Heuristics {
    public:
        Solution neh(std::map<std::string, std::vector<int>>);
        double getSumTime(std::map<std::string, std::vector<int>>, int);
        std::vector<int> sortJobs(std::map<std::string, std::vector<int>>);
};
