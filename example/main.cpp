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
    std::string fileName = argv[1];
    srand(static_cast<unsigned int>(time(0)));
    rand();
    
    std::vector<Vertex> points;
    
    initVetices(points, 3, 12, fileName);
    for(int i = 0; i < 12; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            std::cout << (points[i].getCoordinate())[j] << ", ";
        }
        std::cout << '\n';
    }
    
    Genome best = GA_TSP(points, 3000, 0.4, 1, 100);
    
    return 0;
}
