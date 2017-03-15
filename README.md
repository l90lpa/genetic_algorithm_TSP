# genetic_algorithm_TSP
Genetic algorithm in a header file for finding a good approximate solution to a travelling salesman problem.

## 0. Proposed works

- Convert to templated format

## 1. Folder structure

- GA_TSP.h:
- main.cpp:
- test_main-GA_TSP.cpp:

### 2 Dependencies

- GA_TSP.h: utilizes C++11 features and the standard template library (STL).
- tests/test_main-GA_TSP.cpp: utilizes the Boost Unit Test Framework.

## 3. Setup

### 3.1 Configure

Examples can be seen inside "example/main.cpp" that show the following configuration steps. Inside a relevant C++ file carry out the following:

- Include the header file

\#include "GA_TSP.h"

- seed the pseudo random number generator using srand() and call rand() once.

srand(static_cast<unsigned int>(time(0))); rand();

- create a vector to hold the points (vertice).

std::vector<Vertex> points;

- initialize the points with data from either a txt file.

initVetices(points, short dimensions, short numPoints, std::string fileName);

- OR initialize the points with user input.

initVetices(points, short dimensions, short numPoints);

- run the the genetic algorithm to obtain the best approximate solution Genome

Genome bestGenome = GA_TSP(points, short populationSize, float survivingPopulationPercent, short averageNumMutations, short numGenerations);
