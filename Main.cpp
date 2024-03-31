#include <iostream>

#include "Meta-Heuristics/TabuList.h"

int main() {
    TabuList tl(3);

    Solution sol1({1,2,3,4});
    /*
    Solution sol2({1,2,3});
    Solution sol3({8,9,10});
    std::cout << sol1.getCost() << "\n";
    std::cout << sol2.getCost() << "\n";
    std::cout << sol3.getCost() << "\n";

    std::set<Solution> ints;

    ints.insert(sol1);
    ints.insert(sol2);
    ints.insert(sol3);
    std::cout << "SIZE " << ints.size() << "\n";
    for (Solution i : ints) {
        for (int el : i.getPermutation()) {
            std::cout<< el;
        }
        std::cout << "\n";
    }

    exit(0);
    */

   bool changed = sol1.improve(Search::TABU, Operator::SWAP);

   std::cout << changed;
}