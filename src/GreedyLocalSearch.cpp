#include "../include/GreedyLocalSearch.hpp"

#include <string>
#include <format>
#include <optional>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

string GreedyLocalSearch::getAlgorithmName()
{
    return format("Greedy_{}", neighbourhoodUsed == MoveType::SwapNodes ? "SwapNodes" : "SwapEdges");
}

void GreedyLocalSearch::setMoveSet()
{ 
    moveSet.clear();
    int n = solution.size();
    vector<bool>inSolution(data->numNodes, false);

    // RemoveNode moves - usunięcie wierzchołka solution[node1]
    for (int i = 0; i < n; i++)
    {
        inSolution[solution[i]] = true;
        moveSet.emplace_back(MoveType::RemoveNode, solution[i], i);
    }

    // InsertNode moves - wstawienie wierzchołka data[node1] po wierzchołku solution[node2]
    for (int node1 = 0; node1 < data->numNodes; node1++)
    {
        if (inSolution[node1])
            continue;

        if(n == 0)
        {
            moveSet.emplace_back(MoveType::InsertNode, node1, -1);
            continue;
        }

        for (int node2 = 0; node2 < n; node2++)
        {
            moveSet.emplace_back(MoveType::InsertNode, node1, node2);
        }
    }

  // SwapNodes moves - zamiana wierzchołków solution[node1] i solution[node2] (node1 < node2)
  // or SwapEdges moves- zamiana krawędzi solution[node1] -> solution[node1 + 1] z krawędzią solution[node2] -> solution[node2 + 1] (node1 < node2)
    for (int node1 = 0; node1 < n; node1++)
    {
        for (int node2 = node1 + 1; node2 < n; node2++)
        {
            moveSet.emplace_back(neighbourhoodUsed, node1, node2);
        }
    }
}

optional<Move> GreedyLocalSearch::chooseMove()
{
    vector<int> indices(moveSet.size());
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), rng);
    for(int i : indices)
    {
        Move move = moveSet[i];
        move.deltaScore = calculateDeltaScore(move);
        if(*move.deltaScore > 0)
        {
            return move;
        }
    }
    return nullopt;
}

void GreedyLocalSearch::updateMoveSet(const Move &move)
{
    setMoveSet();
}
