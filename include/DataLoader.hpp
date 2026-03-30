#pragma once

#include <vector>
#include <string>

using namespace std;

class DataLoader
{
public:
    int numNodes;
    vector<int> nodeProfits;
    vector<vector<int>> distanceMatrix;

    DataLoader(const string &filename);
    DataLoader(vector<int> profits, vector<vector<int>> distances) : numNodes(profits.size()), nodeProfits(profits), distanceMatrix(distances) {};
};
