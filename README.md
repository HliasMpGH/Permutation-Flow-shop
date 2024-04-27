# PFS Problem Modelling
This repository holds a model that can be used to produce solutions to the, common in the Operations Reasearch field, [Permutation Flowshop Problem](https://en.wikipedia.org/wiki/Flow-shop_scheduling).

# Contents
## Implementations

The implementation of the models and algorithms can be found in the [source](https://github.com/HliasMpGH/Permutation-Flow-shop/tree/main/src) directory:

- [Heuristics](https://github.com/HliasMpGH/Permutation-Flow-shop/blob/main/src/Heuristics.cpp) - The container of the heuristic methods that are used in the implementation. 

- [Solution](https://github.com/HliasMpGH/Permutation-Flow-shop/blob/main/src/Solution.cpp) - The container of a specific 'Solution' entity. Each final solution produced by the algorithms is an instance of this class.

- [TabuList](https://github.com/HliasMpGH/Permutation-Flow-shop/blob/main/src/TabuList.cpp) - The container of a TabuList that is being used in a [Tabu Search](https://en.wikipedia.org/wiki/Tabu_search).

## Definitions
The definitions of all the entities that are being used in the implementation rest in the [headers](https://github.com/HliasMpGH/Permutation-Flow-shop/tree/main/src/headers) directory:

- [Solution.h](https://github.com/HliasMpGH/Permutation-Flow-shop/blob/main/src/headers/Solution.h) - The definition of a 'Solution' of the problem. Contains all the methods and attributes that a PFSP solution should have.

- [TabuList.h](https://github.com/HliasMpGH/Permutation-Flow-shop/blob/main/src/headers/TabuList.h) - The definition of a TabuList. Contains all the methods and attributes of a List that will be used in a Tabu Seach.

- [Searches.h](https://github.com/HliasMpGH/Permutation-Flow-shop/blob/main/src/headers/Searches.h) - All the available solution exploration algorithms.

- [Operators.h](https://github.com/HliasMpGH/Permutation-Flow-shop/blob/main/src/headers/Operators.h) -
All the available [Local Search](https://en.wikipedia.org/wiki/Local_search_(optimization)) operators.

# Usage
To use this implementation, first compile the project:
> g++ -o solver.exe src/*.cpp

and then execute the solver:
>./solver.exe

The problem input that will be used can be configured in the [data.txt](https://github.com/HliasMpGH/Permutation-Flow-shop/blob/main/data.txt) file. The format of the input has to be in a structure as the following:
```
M1: job1time job2time job3time
M2: job1time job2time job3time
```
**for a 2-machine, 3-job problem.**
# Report
A [brief report](https://github.com/HliasMpGH/Permutation-Flow-shop/blob/main/Report.pdf) also exists, further detailing all of the abovementioned, using a more elegant manner.  