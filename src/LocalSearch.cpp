#include "../include/LocalSearch.hpp"

#include <optional>
#include <vector>
#include <algorithm>
#include <print>
#include <filesystem>
#include <format>
#include <fstream>

using namespace std;
using namespace filesystem;

void LocalSearch::improve()
{
    bool hasImproved = true;
    while (hasImproved)
    {
        hasImproved = false;
        
        optional<Move> bestMove = chooseMove();
        
        if(bestMove)
        {
            hasImproved = true;
            changeSolution(*bestMove);
        }
    }
}

void LocalSearch::changeSolution(const Move &bestMove)
{
    // TODO: Test this function, especially SwapEdges
    switch (bestMove.type)
    {
    case MoveType::InsertNode:
        solution.insert(solution.begin() + *bestMove.node2 + 1, bestMove.node1);
        break;
    case MoveType::RemoveNode:
        solution.erase(solution.begin() + bestMove.node1);
        break;
    case MoveType::SwapNodes:
        swap(solution[bestMove.node1], solution[*bestMove.node2]);
        break;
    case MoveType::SwapEdges:
        reverse(solution.begin() + bestMove.node1 + 1, solution.begin() + *bestMove.node2 + 1);
        break;
    }
}
int LocalSearch::calculateLength()
{
    int score = 0;
    if(solution.size() == 0)
    {
        return score;
    }
    for (size_t currentNode = 1; currentNode < solution.size(); currentNode++)
    {
        // - odległość currentNode - 1 -> currentNode
        score -= data->distanceMatrix[solution[currentNode - 1]][solution[currentNode]];
    }
    if(solution.size() > 1)
    {
        // - odległość lastNode -> currentNode
        score -= data->distanceMatrix[solution[solution.size() - 1]][solution[0]];
    }
    return score;
}

int LocalSearch::calculateScore()
{
    int score = 0;
    if(solution.size() == 0)
    {
        return score;
    }
    // + zysk z odwiedzenia pierwszego wierzchołka
    score = data->nodeProfits[solution[0]];
    for (size_t currentNode = 1; currentNode < solution.size(); currentNode++)
    {
        // - odległość currentNode - 1 -> currentNode
        score -= data->distanceMatrix[solution[currentNode - 1]][solution[currentNode]];
        // + zysk z odwiedzenia wierzchołka currentNode
        score += data->nodeProfits[solution[currentNode]];
    }
    if(solution.size() > 1)
    {
        // - odległość lastNode -> currentNode
        score -= data->distanceMatrix[solution[solution.size() - 1]][solution[0]];
    }
    return score;
}
void LocalSearch::print()
{
    println("Trasa:\n{}",solution);
    println("Funkcja celu: {}", solutionScore);
}
void LocalSearch::saveToFile(const string &filename)
{
    path dir = "../data/solutions";
    create_directories(dir); 
    string fullPath = (dir / format("{}_{}.txt", filename, getAlgorithmName())).string();
    ofstream file(fullPath);
    println(file, "{}", solutionScore);
    for (size_t i = 0; i < solution.size(); i++)
    {
        println(file, "{}", solution[i]);
    }
    file.close();
}