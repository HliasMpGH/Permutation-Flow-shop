#include <iostream>
#include <deque>

#include "Solution.h"


class TabuList {
    private:
        std::deque<Solution> taboos;
    public:
        int size;

        // used to construct new Tabu Lists
        TabuList(int size);

        /* returns the contents of the TabuList */
        std::vector<Solution> getTaboos();

        /* inserts a solution into the TabuList  */
        void tabooMove(Solution);

        /* evaluates if a solution is in the TabuList */
        bool isTaboo(Solution);

};