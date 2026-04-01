#include <string>
#include <vector>
#include <memory>
#include <print>
#include <random>
#include <limits>
#include <ranges>
#include <algorithm>
#include <utility>

#include "../include/DataLoader.hpp"
#include "../include/Solver.hpp"
#include "../include/RandomSolver.hpp"
#include "../include/KRegret.hpp"
#include "../include/LocalSearch.hpp"
#include "../include/RandomLocalSearch.hpp"
#include "../include/GreedyLocalSearch.hpp"

using namespace std;

struct Statistic {
    string data;
    string algorithm;
    double average = 0;
    int min = numeric_limits<int>::max();
    int max = numeric_limits<int>::min();

    Statistic(string data, string algorithm) : data(move(data)), algorithm(move(algorithm)) {}
};

pair<Statistic, Statistic> runTests(unique_ptr<Solver> &solver, int numRuns, mt19937 &rng, const string &data);

void printStatistics(vector<Statistic> &statistics);

int main()
{
    DataLoader dataA("../data/TSPA.csv");
    DataLoader dataB("../data/TSPB.csv");

    int startNode = 0;
    
    vector<unique_ptr<Solver>> solvers;
    solvers.emplace_back(make_unique<RandomSolver>(dataA));
    solvers.emplace_back(make_unique<KRegret>(dataA, startNode, 2));

    // vector<int> solution = {5,2,1,0,8,10};

    // RandomLocalSearch localSearch(dataA, solution, MoveType::SwapNodes, 50);
    
    // int score = localSearch.calculateScore();
    // println("Initial score: {}", score);

    // auto mockedMove = [&score, &localSearch, &solution](Move move) {
    //     move.print();
    //     move.deltaScore = localSearch.calculateDeltaScore(move);
    //     println("Calculated delta score: {}", *move.deltaScore);
    //     score += *move.deltaScore;
    //     localSearch.changeSolution(move);
    //     println("Before:\n{}\n Changed solution:\n{}",solution, localSearch.solution);
    //     println("Score delta calculated: {} / Score: {}", score, localSearch.calculateScore());
    //     solution = localSearch.solution;
    // };


    for (auto &solver : solvers)
    {
        solver->solve();
        solver->saveToFile("DataA");
        println("Solver algorithm: {} on data A", solver->getAlgorithmName());
        solver->print();

        solver->data = &dataB;
    }
    vector<int> solutionR = solvers[0]->solution;
    vector<int> solutionKR = solvers[1]->solution;
    
    vector<unique_ptr<LocalSearch>> localSearches;
    localSearches.emplace_back(make_unique<RandomLocalSearch>(dataA, solutionR, MoveType::SwapNodes, 50));
    localSearches.emplace_back(make_unique<RandomLocalSearch>(dataA, solutionR, MoveType::SwapEdges, 50));
    localSearches.emplace_back(make_unique<GreedyLocalSearch>(dataA, solutionR, MoveType::SwapNodes));
    localSearches.emplace_back(make_unique<GreedyLocalSearch>(dataA, solutionR, MoveType::SwapEdges));

    // DataA
    for (auto &localSearch : localSearches)
    {
        localSearch->improve();
        localSearch->saveToFile("DataA_SolutionR");
        println("Local search algorithm: {} on random solution from data A", localSearch->getAlgorithmName());
        localSearch->print();

        localSearch->solution = solutionKR;
        localSearch->improve();
        localSearch->saveToFile("DataA_SolutionKR");
        println("Local search algorithm: {} on k-regret solution from data A", localSearch->getAlgorithmName());
        localSearch->print();

        localSearch->data = &dataB;
    }

    // DataB
    for (auto &solver : solvers)
    {
        solver->solve();
        solver->saveToFile("DataB");
        println("Solver algorithm: {} on data B", solver->getAlgorithmName());
        solver->print();
    }
    solutionR = solvers[0]->solution;
    solutionKR = solvers[1]->solution;
    
    for (auto &localSearch : localSearches)
    {
        localSearch->solution = solutionR;
        localSearch->improve();
        localSearch->saveToFile("DataB_SolutionR");
        println("Local search algorithm: {} on random solution from data B", localSearch->getAlgorithmName());
        localSearch->print();

        localSearch->solution = solutionKR;
        localSearch->improve();
        localSearch->saveToFile("DataB_SolutionKR");
        println("Local search algorithm: {} on k-regret solution from data B", localSearch->getAlgorithmName());
        localSearch->print();
    }

    // int numRuns = 200;
    // vector<Statistic> lengthStatistics;
    // vector<Statistic> scoreStatistics;
    // mt19937 rng{random_device{}()};
    // // DataA
    // for(auto &solver : solvers)
    // {
    //     auto [statL, statS] = runTests(solver, numRuns, rng, "DataA");
    //     lengthStatistics.push_back(statL);
    //     scoreStatistics.push_back(statS);
    //     solver->data = &dataB;
    // }    
    // // DataB
    // for(auto &solver : solvers)
    // {
    //     auto [statL, statS] = runTests(solver, numRuns, rng, "DataB");
    //     lengthStatistics.push_back(statL);
    //     scoreStatistics.push_back(statS);
    // }

    // println("Length statistics:");
    // printStatistics(lengthStatistics);
    // println("Score statistics:");
    // printStatistics(scoreStatistics);

    return 0;
}
pair<Statistic, Statistic> runTests(unique_ptr<Solver> &solver, int numRuns, mt19937 &rng, const string &data)
{
    Statistic statLength{
        data,
        solver->getAlgorithmName()
    };
    Statistic statScore{
        data,
        solver->getAlgorithmName()
    };

    int maxTestsPossible = min(numRuns, solver->data->numNodes);

    println("Running {} tests on {} with {}", maxTestsPossible, statLength.algorithm, data);

    auto allNodesView = views::iota(0, solver->data->numNodes);

    vector<int> startingNodes(maxTestsPossible);
    ranges::sample(allNodesView, startingNodes.begin(), maxTestsPossible, rng);

    for (int startNode : startingNodes)
    {
        solver->startNode = startNode;
        solver->solve();

        int value = solver->solutionScoreAfterIPhaseI;

        statLength.average += value;
        statLength.min = min(statLength.min, value);
        if(statLength.max < value)
        {
            statLength.max = value;
        }

        value = solver->solutionScore;

        statScore.average += value;
        statScore.min = min(statScore.min, value);
        if(statScore.max < value)
        {
            statScore.max = value;
            solver->saveToFile(data);
        }
    }
    statLength.average /= maxTestsPossible;
    statScore.average /= maxTestsPossible;
    return {statLength, statScore};
}

void printStatistics(vector<Statistic> &statistics)
{
    for(auto &stat : statistics)
    {
        println("Alg: {:<28} | Data: {:<5} | Avg: {:>10.2f} | Min: {:>10} | Max: {:>10}",
            stat.algorithm, stat.data, stat.average, stat.min, stat.max);
    }
}