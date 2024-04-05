#include <iostream>

#include "headers/TabuList.h"

using std::vector;
using std::deque;


TabuList::TabuList(int size) {
    this->size = size;
}

vector<Solution> TabuList::getTaboos() {
    return {taboos.begin(), taboos.end()};
}

void TabuList::tabooMove(Solution move) {
    taboos.push_back(move);

    // if max list size, pop front
    if (taboos.size() > size) {
        taboos.pop_front();
    }
}

bool TabuList::isTaboo(Solution solution) {
    for (Solution taboo : this->getTaboos()) {
        if (taboo == solution) {
            return true;
        }
    }
    return false;
}
