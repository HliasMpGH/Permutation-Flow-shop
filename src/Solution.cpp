#include "headers/Solution.h"
#include "headers/TabuList.h"
#include "headers/Searches.h"
#include "headers/Operators.h"

#include <math.h>
#include <algorithm>

Solution::Solution(const std::map<std::string, std::vector<int>>& times) {
    this->times = times;
}

Solution::Solution(std::vector<int> permutation, const std::map<std::string, std::vector<int>>& times) {
    this->permutation = permutation;
    this->times = times;
}

bool Solution::operator<(const Solution& other) const {
    if (this->getCost() == other.getCost()) {
        return !(*this == other);
    }

    return this->getCost() < other.getCost();
}

bool Solution::operator>(const Solution& other) const {
    return !(*this < other);
}

bool Solution::operator==(const Solution& other) const {
    std::vector<int> thisPermutation = this->getPermutation();
    std::vector<int> otherPermutation = other.getPermutation();

    if (thisPermutation.size() != otherPermutation.size()) {
        return false;
    }

    for (int i = 0; i < thisPermutation.size(); i++) {
        if (thisPermutation[i] != otherPermutation[i]) {
            return false;
        }
    }
    return true;
}

std::vector<int> Solution::getPermutation() const{
    return this->permutation;
}

double Solution::getCost() const {
    std::map<std::string, double> machineTimes;
    double jobTime;

    for (const int job : this->getPermutation()) {
        jobTime = 0;
        for (const auto pair : this->times) {
            double pTime = pair.second.at(job - 1);

            // update the machine
            machineTimes[pair.first] = std::max(jobTime, machineTimes[pair.first]) + pTime;

            // update the job
            jobTime = machineTimes[pair.first];
        }
    }
    return jobTime;
}

void Solution::placeFeature(int position, int solutionFeature) {
    this->permutation.insert(permutation.begin() + position, solutionFeature);
}

void Solution::removeFeature(int position) {
    this->permutation.erase(permutation.begin() + position);
}

int Solution::getBestPosition(int job) {
    int size = this->getPermutation().size();

    // check all possible positions of job in solution and pick the best one
    double minCost = INT_MAX;
    int bestPos = 0;
    for (int pos = 0; pos <= size; pos++) {
        this->placeFeature(pos, job);
        if (this->getCost() < minCost) {
            minCost = this->getCost();
            bestPos = pos;
        }
        this->removeFeature(pos);
    }
    return bestPos;
}

std::set<Solution> Solution::getSwaps() {
    std::vector<int> solution = this->getPermutation();
    std::set<Solution> swaps;

    int size = this->getPermutation().size();

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
        //    std::cout << "swap " << i <<" with " << j;

            std::vector<int> swappedPermutation = solution;

            std::swap(swappedPermutation[i], swappedPermutation[j]);

            Solution neighbor(swappedPermutation, this->times);

        //    std::cout << "cost:" << neighbor.getCost();

            swaps.insert(neighbor);
        }
    }

    return swaps;
}

std::set<Solution> Solution::getRels() {
    std::vector<int> solution = this->getPermutation();
    std::set<Solution> relocations;

    int size = this->getPermutation().size();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {

            // cant relocate element on the same position
            if (i == j || j == i + 1) {
                continue;
            }

            // make sure to count for position shifts
            int relocatedPosition = (i < j ? i : i + 1);

            // std::cout << "relocate " << i << " on " << j << std::endl;

            std::vector<int> relocatedPermutation = permutation;

            // place i on position j
            relocatedPermutation.insert(relocatedPermutation.begin() + j, relocatedPermutation[i]);

            // remove element from old position
            relocatedPermutation.erase(relocatedPermutation.begin() + relocatedPosition);

            Solution neighbor(relocatedPermutation, this->times);

            relocations.insert(neighbor);
        }
    }

    return relocations;
}

std::set<Solution> Solution::produceNeighborhood(Operator op) {
    // the final neighborhood
    std::set<Solution> neighborhood;

    switch (op) {
    case Operator::SWAP:
        neighborhood.merge(getSwaps());
        break;
    case Operator::RELOC:
        // produce relocation neighborhood
        neighborhood.merge(getRels());
        break;
    }

    return neighborhood;
}

void Solution::transformTo(Solution &other) {
    this->permutation = other.getPermutation();
}

bool Solution::move(std::set<Solution> neighborhood, Search searchType) {
    if (neighborhood.size() == 0) {
        std::cout << "no more neighbors" << std::endl;
        return false;
    }

    // the neighbor solution with the least cost
    Solution bestNeighbor = *neighborhood.begin();

    std::cout << "cost: " << bestNeighbor.getCost() << std::endl;
    switch (searchType) {
    case Search::BASIC:
        // basic local search schema

        if (*this > bestNeighbor) {
            // if the neighbor is better than the current
            // solution, transform to neighbor
            this->transformTo(bestNeighbor);
            return true;
        }

        return false;
    case Search::TABU:
        // Tabu Search

        // hill climb to the best neighbor
        this->transformTo(bestNeighbor);

        return true; // a move will always be possible
    case Search::ANNEALING:
        // Simulated Annealing - currently unsupported

        break;
    }

    return false;
}

void Solution::performBasic(Operator op) {
    while (true) {
        std::set<Solution> neighborhood = this->produceNeighborhood(op);

        if (!this->move(neighborhood, Search::BASIC)) {
            break;
        }
    }
}

void Solution::performTabu(Operator op) {
    // the best solution found during the search
    Solution bestSolution = *this;


    // number of iterations
    int end = 10;

    TabuList tabulist(7);

    int current_iteration = 0;
    while (current_iteration++ != end) {
        // the solution we are currently iterating
        Solution current_solution = *this;
        std::cout << "iteration " << current_iteration << std::endl;
        std::set<Solution> neighborhood = this->produceNeighborhood(op);
        std::set<Solution> filteredNeighborhood;

        // remove taboo solutions
        for (Solution neighbor : neighborhood) {

            if (tabulist.isTaboo(neighbor)) {
                continue;
            }

            filteredNeighborhood.insert(neighbor);
        }

        // if a move was made, taboo the previous solution
        if (this->move(filteredNeighborhood, Search::TABU)) {
            tabulist.tabooMove(current_solution);

            // record best solution
            if (*this < bestSolution) {
                bestSolution = *this;
            }
        }
    }

    // change this solution to the best solution found in the search
    if (!(*this == bestSolution)) {
        this->transformTo(bestSolution);
    }
}

void Solution::performSA(Operator op) {

}

bool Solution::improve(Search searchType, Operator op) {
    Solution oldSolution = *this;

    switch (searchType) {
    case Search::BASIC:
        this->performBasic(op);
        break;
    case Search::TABU:
        this->performTabu(op);
        break;
    case Search::ANNEALING:
        this->performSA(op);
        break;
    }

    return !(oldSolution == *this); 
}
