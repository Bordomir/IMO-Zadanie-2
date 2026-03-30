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
    implement setMoveSet
    implement chooseMove
    implement updateMoveSet

- CLass SimpleLocalSearch
    implement setMoveSet
    implement chooseMove
    implement updateMoveSet




### OPTIONAL:
- Class SolutionHolder
    inherited by solvers,
    calculates solution,
    prints solution,
    saves solution to file,