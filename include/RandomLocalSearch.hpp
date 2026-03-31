#pragma once

#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <optional>

#include "LocalSearch.hpp"
#include "DataLoader.hpp"

using namespace std;

class RandomLocalSearch : public LocalSearch
{
public:
    vector<int> bestSolution;
    int bestSolutionScore;
    mt19937 rng;
    long long timeLimit;
    chrono::time_point<chrono::high_resolution_clock> startTime;
    vector<int> inSolution;


    RandomLocalSearch(DataLoader &data, vector<int> solution, MoveType neighbourhood, long long timeLimit, unsigned int seed = random_device{}()) : 
        LocalSearch(data, solution, neighbourhood), 
        bestSolution(solution), 
        bestSolutionScore(calculateScore()),
        rng(seed),
        timeLimit(timeLimit) {};
    string getAlgorithmName() override;
    void setMoveSet() override;
    optional<Move> chooseMove() override;
    void updateMoveSet(const Move &move) override;
private:
    int randomInt(int min, int max);
};