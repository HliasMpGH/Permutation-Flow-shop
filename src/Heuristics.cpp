#include "headers/Heuristics.h"

Solution Heuristics::neh(std::map<std::string, std::vector<int>> times) {
    int jobsNum = times["M1"].size();

    if (jobsNum < 2) {
        return Solution({1}, times);
    }

    std::vector<int> sortedJobs = sortJobs(times);
    Solution finalSolution(times);

    // sort the first 2 jobs optimally
    Solution first({1, 2}, times);
    Solution second({2, 1}, times);

    if (second < first) {
        finalSolution = second;
    } else {
        finalSolution = first;
    }

    for (int job = 3; job <= jobsNum; job++) {
        int pos = finalSolution.getBestPosition(job);
        finalSolution.placeFeature(pos, job);
    }

    return finalSolution;
}

double Heuristics::getSumTime(std::map<std::string, std::vector<int>> times, int job) {
    double cost = 0;
    for (const auto &pair : times) {
        cost += pair.second.at(job - 1);
    }
    return cost;
}

std::vector<int> Heuristics::sortJobs(std::map<std::string, std::vector<int>> times) {
    int jobsNum = times["M1"].size();
    // the starting job sequence
    std::vector<int> jobs;
    jobs.insert(jobs.begin(), 1);

    for (int job = 1; job <= jobsNum; job++) {
        double sumTime = getSumTime(times, job);
        int s = 0;
        for (int j : jobs) {
            if (getSumTime(times, j) < getSumTime(times, job)) {
                break;
            }
            s += 1;
        }
        jobs.insert(jobs.begin() + s, job);
    }
    return jobs;
}
