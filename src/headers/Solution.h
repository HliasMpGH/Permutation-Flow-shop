#pragma once

#include <iostream>
#include <vector>
#include <set>

#include <map>
#include <list>

#include "Operators.h"
#include "Searches.h"

class Solution {
    private:
        // the permutation of the solution
        std::vector<int> permutation;

        // the cost map that this solution will consider
        std::map<std::string, std::vector<int>> times;

        // returns all the unique swaps of solution
        std::set<Solution> getSwaps();

        // returns all the unique relocations of solution
        std::set<Solution> getRels();

        // transforms this solution into a new one
        void transformTo(Solution &other);

        void performBasic(Operator op);

        void performTabu(Operator op);

        void performSA(Operator op);
    public:
        // used to construct new solutions
        Solution(const std::map<std::string, std::vector<int>> &times);

        Solution(std::vector<int> permutation, const std::map<std::string, std::vector<int>> &times);

        // used to retrieve the permutation of a solution
        std::vector<int> getPermutation() const;

        // used to calculate and return the cost of a solution
        double getCost() const;

        // places a solution feature to a certain position of this solution
        void placeFeature(int position, int feature);

        // removes a solution feature from a certain position of this solution
        void removeFeature(int position);

        // returns the best position of a job in this solution
        int getBestPosition(int job);

        // produces the neighborhood of this solution
        // based on an LS operator
        std::set<Solution> produceNeighborhood(Operator op);

        // makes a move on the solution based on a neighborhood
        bool move(std::set<Solution> neighborhood, Search searchType);

        // improves the solution using a local search schema
        // and a move type. Returns true if it succeeded
        bool improve(Search searchType, Operator op);

        // operators
        bool operator<(const Solution& other) const;

        bool operator>(const Solution& other) const;

        bool operator==(const Solution& other) const;
};
