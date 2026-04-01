#include "../include/RandomLocalSearch.hpp"

#include <string>
#include <format>
#include <vector>
#include <optional>

using namespace std;

string RandomLocalSearch::getAlgorithmName()
{
    return format("Random_{}", neighbourhoodUsed == MoveType::SwapNodes ? "SwapNodes" : "SwapEdges");
}
void RandomLocalSearch::setMoveSet()
{
    bestSolution = solution;
    bestSolutionScore = solutionScore;
    startTime = chrono::high_resolution_clock::now();
    inSolution = vector<int>(data->numNodes, -1);
    for (size_t i = 0; i < solution.size(); i++)
        inSolution[solution[i]] = i;
}
optional<Move> RandomLocalSearch::chooseMove()
{
    // Sprawdzenie limitu czasu
    auto currentTime = chrono::high_resolution_clock::now();
    auto elapsedTime = chrono::duration<double, std::milli>(currentTime-startTime).count();
    if (elapsedTime >= timeLimit)
    {
        solution = bestSolution;
        solutionScore = bestSolutionScore;
        return nullopt;
    }

    // Zliczanie liczby wszystkich ruchów w celu znormalizowania prawdopodobieństwa wyboru ruchu zmieniającego liczbę wierzchołków i ruchu zamieniającego kolejność wierzchołków
    int totalMoves = data->numNodes + (solution.size() * (solution.size() - 1)) / 2; 

    bool isChangingNodeCount = randomInt(1, totalMoves) <= data->numNodes;
    if (solution.size() < 4)
        isChangingNodeCount = true;

    MoveType bestMoveType;
    int node1;
    optional<int> node2;
    if (isChangingNodeCount)
    {
        node1 = randomInt(0, data->numNodes - 1);
        while (solution.size() < 2 && inSolution[node1] >= 0)
            node1 = randomInt(0, data->numNodes - 1);

        if (inSolution[node1] >= 0)
        {
            bestMoveType = MoveType::RemoveNode;
            node2 = inSolution[node1];
        }
        else
        {
            bestMoveType = MoveType::InsertNode;
            if(solution.size() == 0)
                node2 = -1;
            else
                node2 = randomInt(0, solution.size() - 1);
        }
    }
    else
    {
        bestMoveType = neighbourhoodUsed;
        node1 = randomInt(0, solution.size() - 1);
        node2 = randomInt(0, solution.size() - 1);
        while (*node2 == node1)
            node2 = randomInt(0, solution.size() - 1);
        if (node1 > *node2)
            swap(node1, *node2);
    }

    Move bestMove(bestMoveType, node1, node2);
    bestMove.deltaScore = calculateDeltaScore(bestMove);
    return bestMove;
}

void RandomLocalSearch::updateMoveSet(const Move &move)
{
    if (solutionScore > bestSolutionScore)
    {
        bestSolution = solution;
        bestSolutionScore = solutionScore;
    }

    const int n = solution.size();
    switch (move.type)
    {
        case MoveType::InsertNode:
        {
            int newNodeSolutionIndex = *move.node2 + 1;
            inSolution[move.node1] = newNodeSolutionIndex++;
            for (; newNodeSolutionIndex < n; newNodeSolutionIndex++)
                inSolution[solution[newNodeSolutionIndex]]++;
            break;
        }
        case MoveType::RemoveNode:
        {
            int removedNodeSolutionIndex = *move.node2;
            inSolution[move.node1] = -1;
            for (; removedNodeSolutionIndex < n; removedNodeSolutionIndex++)
                inSolution[solution[removedNodeSolutionIndex]]--;
            break;
        }
        case MoveType::SwapNodes:
        {
            inSolution[solution[move.node1]] = move.node1;
            inSolution[solution[*move.node2]] = *move.node2;
            break;
        }
        case MoveType::SwapEdges:
        {
            for (int i = move.node1 + 1; i <= *move.node2; i++)
            {
                inSolution[solution[i]] = i;
            }
            break;
        }
    }
}

int RandomLocalSearch::randomInt(int min, int max)
{
    uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}
