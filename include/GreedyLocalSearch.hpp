#pragma once

#include <vector>
#include <string>
#include <optional>
#include <random>

#include "LocalSearch.hpp"
#include "DataLoader.hpp"

using namespace std;

class GreedyLocalSearch : public LocalSearch
{
public:
    mt19937 rng;

    GreedyLocalSearch(DataLoader &data, vector<int> solution, MoveType neighbourhood, unsigned int seed = random_device{}()) : LocalSearch(data, solution, neighbourhood), rng(seed) {};
    string getAlgorithmName() override;
    void setMoveSet() override;
    optional<Move> chooseMove() override;
    void updateMoveSet(const Move &move) override;
};