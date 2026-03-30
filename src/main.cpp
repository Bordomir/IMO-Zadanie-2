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
    solvers.emplace_back(make_unique<KRegret>(dataA, startNode, 2, -0.2));

    int numRuns = 200;
    vector<Statistic> lengthStatistics;
    vector<Statistic> scoreStatistics;
    mt19937 rng{random_device{}()};
    // DataA
    for(auto &solver : solvers)
    {
        auto [statL, statS] = runTests(solver, numRuns, rng, "DataA");
        lengthStatistics.push_back(statL);
        scoreStatistics.push_back(statS);
        solver->data = &dataB;
    }    
    // DataB
    for(auto &solver : solvers)
    {
        auto [statL, statS] = runTests(solver, numRuns, rng, "DataB");
        lengthStatistics.push_back(statL);
        scoreStatistics.push_back(statS);
    }

    println("Length statistics:");
    printStatistics(lengthStatistics);
    println("Score statistics:");
    printStatistics(scoreStatistics);

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