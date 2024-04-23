#include <map>
#include <string>
#include <vector>

#include "Solution.h"

class Heuristics {
    public:
        static Solution neh(std::map<std::string, std::vector<int>>);

    private:
        static double getSumTime(std::map<std::string, std::vector<int>>, int);
        static std::vector<int> sortJobs(std::map<std::string, std::vector<int>>);
};
