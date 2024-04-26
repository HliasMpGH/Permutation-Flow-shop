#include <iostream>
#include <fstream>
#include <regex>
#include <list>
#include <sstream>
#include <map>

#include "headers/TabuList.h"
#include "headers/Solution.h"
#include "headers/Heuristics.h"

// map of the processing times of the problem
std::map<std::string, std::vector<int>> processTimes;

std::map<std::string, std::vector<int>> getTimesInput(std::string);
std::vector<int> extractData(std::string);

int main() {
    // modify accordingly
    std::string dataInputFile = "data.txt";

    processTimes = getTimesInput(dataInputFile);

}

std::map<std::string, std::vector<int>> getTimesInput(std::string fileName) {
    std::ifstream file(fileName);
    std::string dataLine;

    std::map<std::string, std::vector<int>> processTimes;
    if (file.is_open()) {
        while (file) {
            std::getline(file, dataLine);

            // skip comments and empty lines
            if (std::regex_search(dataLine, std::regex("^\\s*#")) ||
                std::regex_replace(dataLine, std::regex("\\s+"), "").empty()) {
                continue;
            }

            // the border between the machine-jobs' processing times
            int border = dataLine.find(':');

            if (border == std::string::npos) {
                throw std::runtime_error("Invalid Data from Input: " + dataLine);
            }

            std::string machine = dataLine.substr(0, border);
            std::string jobsTimes = dataLine.substr(border + 1);

            processTimes[machine] = extractData(jobsTimes);
        }
    } else {
        throw std::runtime_error("Error - Cant Open Input File: " + fileName);
    }

    return processTimes;
}

// extracts a list of ints from a string of process times
std::vector<int> extractData(std::string dataLine) {
    std::vector<int> times;
    std::istringstream iss(dataLine);

    int time;
    while (iss >> time) {
        times.push_back(time);
    }

    return times;
}
