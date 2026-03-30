#pragma once

#include <vector>
#include <string>

#include "DataLoader.hpp"

using namespace std;

class LocalSearch
{
public:
    DataLoader *data;
    vector<int> solution;
    int solutionScore;

    LocalSearch(DataLoader &data, vector<int> solution) : data(&data), solution(solution), solutionScore(0) {};
    virtual string getAlgorithmName() = 0;
    virtual void improve() = 0;    
    int calculateLength();
    int calculateScore();
    void print();
    void saveToFile(const string &filename = "");
};