//
//  main.cpp
//  GA_TSP
//
//  Created by Liam on 05/03/2017.
//  Copyright © 2017 Liam. All rights reserved.
//

#include <iostream>
#include "GA_TSP.h"

int main(int argc, const char * argv[])
{
    // COMMON SETUP
    srand(static_cast<unsigned int>(time(0)));
    rand();
    
    // SETUP for taking in the data from a CSV file where each row is a vertex (point)
    std::string fileName = argv[1];
    std::vector<Vertex> pointsOne;
    initVetices(pointsOne, 3, 12, fileName);
 
    Genome bestOne = GA_TSP(pointsOne, 3000, 0.4, 1, 100);
    
    // SETUP for taking in the data from user input via std::cin
    std::vector<Vertex> pointsTwo;
    initVetices(pointsTwo, 2, 6);
 
    Genome bestTwo = GA_TSP(pointsTwo, 100, 0.3, 0.7, 100);
    
    
    return 0;
}
