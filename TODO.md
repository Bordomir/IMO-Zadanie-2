### TODO:
- Class LocalSearch
    inherited by LocalSearch algorithms
    shares common template for improving solutions which looks like this:
        Virtual Method setMoveSet()
        Loop
            Virtual Method chooseMove
            if found move:
                changeSolution(move)
                Virtual Method updateMoveSet(move)
        Until improvement is done


- Class RandomLocalSearch
    while loop depends on passed time
    implement setMoveSet
    implement chooseMove
    implement updateMoveSet
DONE weird solutions on SwapNode

- CLass GreedyLocalSearch
    randomized process of checking moves, chooses first that is positive
    implement setMoveSet
    implement chooseMove
    implement updateMoveSet
DONE seems fine maybe a little weird on SwapNode also 

- CLass SteepLocalSearch
    checks all moves, chooses best one
    implement setMoveSet
    implement chooseMove
    implement updateMoveSet

- Improvement calculated on score delta instead of calcualting score of a solution

- Experiment
    Run 100 times on every data instance
    



### OPTIONAL:
- Optimized moves calculation
    keeping calculated deltaScore of moves in memory (matrix per move type (insert/remove and swap) using data indexes and not solution indexes)
    usefull to have another vector with mapping of every node to its position in solution that also has to be updated during move execution
    updating values of deltaScore after executing move

- Class SolutionHolder
    inherited by solvers,
    calculates solution,
    prints solution,
    saves solution to file,