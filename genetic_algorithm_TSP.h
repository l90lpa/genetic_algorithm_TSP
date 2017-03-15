//
//  genetic_algorithm_TSP.h
//  genetic_algorithm_TSP
//
//  Created by Liam on 07/03/2017.
//  Copyright © 2017 Liam. All rights reserved.
//

#ifndef genetic_algorithm_TSP_h
#define genetic_algorithm_TSP_h


#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm> //sort()
#include <random> //srand() rand()
#include <ctime> //time()
#include <set> //std::set<>
#include <fstream>




short randNumGen(short min, short max) //inclusive range
{
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<short>((fraction * (max - min + 1) * rand()) + min);
}


// Hold the location the location of a vertex in Cartesian coordinates
class Vertex
{
private:
    short m_vIndex;
    short m_dimension;
    std::vector<float> m_vCoord;
public:
    Vertex(short vIndex, std::vector<float> vCoord) : m_vIndex{vIndex}, m_vCoord{vCoord}
    {
        m_dimension = vCoord.size();
    }
    const std::vector<float>& getCoordinate() const
    {
        return m_vCoord;
    }
    short getDimension() const
    {
        return m_dimension;
    }
};

// Initialize a series of points with std::cin user input.
void initVetices(std::vector<Vertex> &point, short dimension, short numPoints, std::istream& inStream = std::cin)
{
    point.reserve(numPoints);
    for(short i = 0; i < numPoints; ++i)
    {
        std::vector<float> tempCoord;
        for(short j = 0;  j != dimension; ++j)
        {
            float temp;
            std::cout << "Enter point[" << i << "] x values: ";
            inStream >> temp;
            tempCoord.push_back(temp);
        }
        Vertex tempVertex(i, tempCoord);
        point.push_back(tempVertex);
    }
}

// Initialize a series of points with file input.
void initVetices(std::vector<Vertex> &point, short dimension, short numPoints, const std::string file_name)
{
    std::ifstream file;
    file.open(file_name);
    if(!file.is_open())
    {
        std::string message{"error when opening file, "};
        message += std::strerror(errno);
        throw std::runtime_error(message);
    }
    
    point.reserve(numPoints);
    for(int i = 0; i != numPoints; ++i)
    {
        std::vector<float> tempCoord;
        for(short j = 0; j < dimension; ++j)
        {
            std::string temp;
            if(!std::getline(file, temp, ','))
            {
                std::string message{"error when reading line, "};
                message += std::strerror(errno);
                throw std::runtime_error(message);
            }
            else
            {
                tempCoord.push_back(static_cast<float>(std::stod(temp)));
            }
        }
        Vertex tempVertex(i, tempCoord);
        point.push_back(tempVertex);
    }
    file.close();
}


// Genome holds one possible path throught the vertices, and is used to calculate
// to the path length and mutate the genome.
class Genome
{
private:
    short m_numVertices;
    std::vector<short> m_orderVertices;
    double m_pathLength;
public:
    Genome(short numVertices) : m_numVertices{numVertices}, m_pathLength{-1}
    {
        for(short i = 0; i != m_numVertices; ++i)
        {
            m_orderVertices.push_back(i);
        }
        std::random_device seed;
        std::mt19937 randGen(seed());
        std::shuffle(m_orderVertices.begin(), m_orderVertices.end(), randGen);
    }
    
    Genome(short numVertices, std::vector<short> orderedGenome) : m_numVertices{numVertices}, m_orderVertices{orderedGenome}, m_pathLength{-1}
    {
    }
    
    void computePathLength(const std::vector<Vertex> &point)
    {
        m_pathLength = 0;
        for(short i = 0; i != m_numVertices; ++i)
        {
            short j = (i + 1) % m_numVertices;
            float sqLength = 0;
            std::vector<float> A = point[m_orderVertices[i]].getCoordinate();
            std::vector<float> B = point[m_orderVertices[j]].getCoordinate();
            
            for(short k = 0; k != point[0].getDimension(); ++k)
            {
                sqLength += std::pow(A[k] - B[k], 2.0);
            }
            m_pathLength += std::pow(sqLength, 0.5);
        }
    }
    
    float getPathLength() const
    {
        return m_pathLength;
    }
    
    std::vector<short> getOrder() const
    {
        return m_orderVertices;
    }
    
    short getNumVertice() const
    {
        return m_numVertices;
    }
    
    void mutate()
    {
        short geneOne = randNumGen(0, m_numVertices - 1);
        short geneTwo;
        do
        {
            geneTwo = randNumGen(0, m_numVertices - 1);
        }
        while(geneOne == geneTwo);
        std::swap(m_orderVertices[geneOne], m_orderVertices[geneTwo]);
    }
};


// shorterGenomeCheck function: used inside GA_TSP in the sorting algorithm to check if genome A
//                              has a longer path than genome B.

bool shorterGenomeCheck(const Genome& A, const Genome& B)
{
    return A.getPathLength() < B.getPathLength();
}


//crosslink function: Modified single point crossover method: create a new genome with the
//                    first 0 to crossPoint - 1 elements taken from A, then the remaining
//                    distict elements taken from B in verticesOrder.

Genome crossover(const Genome& A, const Genome& B)
{
    std::set<short> checkSet; // used to ensure that the new Genome object is filled with distict vertice.
    std::vector<short> newGenomeOrder;
    short crossPoint = randNumGen(1, B.getNumVertice() - 1); // To ensure that atleast one number is changed but
                                                             // also stop the entire Genomes from simply being swapped.
    for(short i = 0; i < crossPoint; ++i)
    {
        newGenomeOrder.push_back((A.getOrder())[i]);
        checkSet.insert((A.getOrder())[i]);
    }
    for(short i = 0; i < B.getNumVertice(); ++i)
    {
        if(checkSet.insert((B.getOrder())[i]).second == true)
            newGenomeOrder.push_back((B.getOrder())[i]);
    }
    Genome C{B.getNumVertice(), newGenomeOrder};
    return C;
}

//GA_TSP algorithm: - must call srand() prior to running this algorithm.
//                  - survivingPopulationPercent is the percentage of the population to be remain unchanged
//                    after crossover. Currently this is sorted on fittest (shortest path).
//                  - averageMutationsis the average number of mutations an individual in the population will
//                    experience at each generation. Since the randNumGen is uniformly distributed we only need
//                    to find the total number of mutations that would to occur if the the population did on
//                    average experince this rate of mutation and then use the randNumGen to randomly distribute
//                    the total number of mutations amongst the population to simulate this.

Genome GA_TSP(const std::vector<Vertex> &points, short populationSize, float survivingPopulationPercent, int averageMutations, int numGenerations)
{
    int survivingPopulation = static_cast<int>(floorf(static_cast<float>(populationSize) * survivingPopulationPercent));
    int numMutations = populationSize * averageMutations;
    
    std::vector<Genome> population;
    short numVertices = points.size();
    
    
    for(int i = 0; i != populationSize; ++i)
    {
        Genome temp(numVertices);
        temp.computePathLength(points);
        population.push_back(temp);
    }
    
    std::sort(population.begin(), population.end(), shorterGenomeCheck);
    int generation = 0;
    
    while(generation != numGenerations)
    {
        for(int i = 0; i != numMutations; ++i)
        {
            population[randNumGen(1, populationSize - 1)].mutate();
        }
        
        for(short i = survivingPopulation + 1; i != populationSize; ++i)
        {
            short A = randNumGen(0, survivingPopulation);
            short B = randNumGen(1, survivingPopulation);
            population[i] = crossover(population[A], population[B]);
        }
        
        for(auto &x : population)
        {
            x.computePathLength(points);
        }
        
        std::sort(population.begin(), population.end(), shorterGenomeCheck);
        
        if(generation % 10 == 0)
        {
            const std::vector<short> bestGenome = population[0].getOrder();
            std::cout << "Generation " << generation << ": [";
            for(int j = 0; j != bestGenome.size(); ++j)
            {
                std::cout << bestGenome[j];
                if(j != bestGenome.size() - 1)
                    std::cout << ", ";
            }
            std::cout << "]\n" << "Path length = " << population[0].getPathLength() << '\n';
        }
        ++generation;
    }
    --generation;
    const std::vector<short> bestGenome = population[0].getOrder();
    std::cout << "Generation " << generation << ": [";
    for(int j = 0; j != bestGenome.size(); ++j)
    {
        std::cout << bestGenome[j];
        if(j != bestGenome.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]\n";
    return population[0];
}



#endif /* genetic_algorithm_TSP_h */
