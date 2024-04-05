#include "headers/Solution.h"
#include "headers/TabuList.h"
#include "headers/Searches.h"
#include "headers/Operators.h"

#include <math.h>

Solution::Solution() {
    // empty constructor
}

Solution::Solution(std::vector<int> permutation) {
    this->permutation = permutation;
}

bool Solution::operator<(const Solution& other) const {
    if (this->getCost() == other.getCost()) {
        // if 2 solutions have the same cost, sort based
        return !(*this == other);
    }

    return this->getCost() < other.getCost();
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
    double cost = 0;
    for (int var : this->getPermutation()) {
        cost += var;
    }
    return cost;
}

void Solution::appendFeature(int solutionFeature) {
    this->permutation.push_back(solutionFeature);
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

            Solution neighbor(swappedPermutation);

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

            Solution neighbor(relocatedPermutation);

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
    // find min solution in neighborhood
    // make the move if less than getCost()
    // true if it made the move

    if (neighborhood.size() == 0) {
        std::cout << "no more neighbors" << std::endl;
        return false;
    }

    // the neighbor solution with the least cost
    Solution bestNeighbor = *neighborhood.rbegin();
    std::cout << "move to ";

    for (int s : bestNeighbor.getPermutation()) {
        std::cout << s << " ";
    }

    std::cout << "cost: " << bestNeighbor.getCost() << std::endl;
    switch (searchType) {
    case Search::BASIC:
        // basic local search schema

        if (bestNeighbor < *this) {
            std::cout << "found better cost by "<<bestNeighbor.getPermutation()[0] << bestNeighbor.getPermutation()[1] << bestNeighbor.getPermutation()[2] << bestNeighbor.getPermutation()[3]<< " with cost " << bestNeighbor.getCost() << "\n";
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
        // Simulated Annealing

        if (bestNeighbor < *this) {
            this->transformTo(bestNeighbor);
            return true;
        }

        // calculate θt
        double th;

        double prob = pow(10, -(bestNeighbor.getCost() - this->getCost()) / th);

        // make the move depending on probability
        break;
    }

    return false;
}

void Solution::performBasic(Operator op) {

}

void Solution::performTabu(Operator op) {
    // the best solution found during the search
    Solution bestSolution = *this;


    // number of iterations
    int end = 10;

    TabuList tabulist(7);

    int current_iteration = 0;
    while (current_iteration++ != end) {
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

        // if a move was made, taboo it
        if (this->move(filteredNeighborhood, Search::TABU)) {
            tabulist.tabooMove(*this);

            // record best solution
            if (*this < bestSolution) {
                bestSolution = *this;
            }
        }

        std::cout << "taboos: " << std::endl;
        for (Solution cont : tabulist.getTaboos()) {
            for (int s : cont.getPermutation()) {
                std::cout << "\t" << s << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "BEST SO FAR: ";

        for (int cont : bestSolution.getPermutation()) {
            std::cout << cont;
        }
        std::cout << std::endl;
    }
}

void Solution::performSA(Operator op) {

}

bool Solution::improve(Search searchType, Operator op) {
    Solution oldSolution = *this;

    //Solution sol({1,2,3,4});
    //this->transformTo(sol);


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