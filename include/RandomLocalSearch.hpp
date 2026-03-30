#pragma once

#include <vector>
#include <string>

#include "LocalSearch.hpp"
#include "DataLoader.hpp"

using namespace std;

class RandomLocalSearch : public LocalSearch
{
public:
    RandomLocalSearch(DataLoader &data, vector<int> solution) : LocalSearch(data, solution) {};
    string getAlgorithmName() override;
    optional<Move> chooseMove() override;
};