#pragma once

#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <optional>
#include <memory>

#include "LocalSearch.hpp"
#include "DataLoader.hpp"
#include "Solver.hpp"

using namespace std;

class RandomLocalSearch : public LocalSearch
{
public:
    mt19937 rng;
    double timeLimit;
    vector<int> bestSolution;
    int bestSolutionScore;
    chrono::time_point<chrono::high_resolution_clock> startTime;
    vector<int> inSolution;

    RandomLocalSearch(unique_ptr<Solver> &solver, MoveType neighbourhood, double timeLimit, unsigned int seed = random_device{}()) : 
        LocalSearch(solver, neighbourhood), 
        rng(seed), 
        timeLimit(timeLimit) {};
    RandomLocalSearch(DataLoader &data, vector<int> solution, MoveType neighbourhood, double timeLimit, unsigned int seed = random_device{}()) : 
        LocalSearch(data, solution, neighbourhood),
        rng(seed),
        timeLimit(timeLimit) {};
    string getAlgorithmName() override;
    void setMoveSet() override;
    optional<Move> chooseMove() override;
    void updateMoveSet(const Move &move) override;
private:
    int randomInt(int min, int max);
};