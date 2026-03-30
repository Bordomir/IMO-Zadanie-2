#pragma once

#include <optional>
#include <vector>
#include <string>

#include "DataLoader.hpp"

using namespace std;

enum class MoveType
{
    InsertNode, // wstawienie wierzchołka data[node1] po wierzchołku solution[node2]
    RemoveNode, // usunięcie wierzchołka solution[node1]
    SwapNodes, // zamiana wierzchołków solution[node1] i solution[node2]
    SwapEdges // zamiana krawędzi solution[node1] -> solution[node1 + 1] z krawędzią solution[node2] -> solution[node2 + 1]
};

struct Move {
    MoveType type;
    int deltaScore;
    int node1;
    optional<int> node2;

    Move(MoveType type,int deltaScore, int node1, optional<int> node2 = nullopt) : type(type), deltaScore(deltaScore), node1(node1), node2(node2) {}
};

class LocalSearch
{
public:
    DataLoader *data;
    vector<int> solution;
    int solutionScore;

    LocalSearch(DataLoader &data, vector<int> solution) : data(&data), solution(solution), solutionScore(0) {};
    virtual string getAlgorithmName() = 0;
    void improve();
    virtual optional<Move> chooseMove() = 0;
    void changeSolution(const Move &move);

    int calculateLength();
    int calculateScore();
    void print();
    void saveToFile(const string &filename = "");
};