//
//  test_main-GA_TSP.cpp
//  TEST-GA_TSP
//
//  Created by Liam on 15/03/2017.
//  Copyright © 2017 Liam. All rights reserved.
//

#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE Test: GA_TSP



#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <iostream>
#include <array>
#include "GA_TSP.h"



//For use in Player class, test suite.
struct Fixture
{

};


BOOST_AUTO_TEST_SUITE(test_suite_GA_TSP)

//This is an alternative example of testing the that data output is correct. It seems to work fine for
//explicit output_test_stream tests but when used with the pattern_match style test as used in the
//Test Suite: deck, Test Case: printDeck(), it detects the errors fine but doesn't provide feedback as
//to which test case failed or information about the failure.
BOOST_AUTO_TEST_CASE(Vertex_instansiate_getMethods)
{
    std::vector<float> coord{3.141, 2.718};
    Vertex A{10, coord};
    
    BOOST_REQUIRE_EQUAL(A.getDimension(), 2);
    BOOST_REQUIRE_CLOSE_FRACTION((A.getCoordinate())[0], 3.141, 0.0005);
}

BOOST_AUTO_TEST_CASE(initVertices_userInput)
{
    std::stringstream userInput;
    userInput << "3.141 2.718 1.141 0.5 0.6 60";
    userInput << '\n';
    std::vector<Vertex> points;
    initVetices(points, 2, 3, userInput);
    BOOST_REQUIRE_EQUAL(points.size(), 3);
    
    std::vector<float> B = points[2].getCoordinate();
    BOOST_REQUIRE_CLOSE_FRACTION(B[0], 0.6, 0.05);
    BOOST_REQUIRE_CLOSE_FRACTION(B[1], 60, 0.05);
}

BOOST_AUTO_TEST_CASE(initVertices_fileInput)
{
    boost::test_tools::output_test_stream output("/Users/nick/Desktop/test12OutputPattern.txt", true);
    std::vector<Vertex> points;
    initVetices(points, 3, 12, "/Users/nick/Desktop/test12.txt");
    BOOST_REQUIRE_EQUAL(points.size(), 12);
    
    for(int i = 0; i < 12; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            output << (points[i].getCoordinate())[j] << ", ";
        }
        output << '\n';
    }
    
    BOOST_CHECK(output.match_pattern());
}

BOOST_AUTO_TEST_CASE(Genome_instansiation)
{
    srand(static_cast<unsigned int>(time(0)));
    rand();
    std::vector<Vertex> points;
    initVetices(points, 3, 12, "/Users/nick/Desktop/test12.txt");
    Genome C{12};
    BOOST_CHECK_EQUAL(C.getNumVertice(), 12);

    std::vector<short> C_order = C.getOrder();
    for(short i = 0; i < C.getNumVertice(); i++)
    {
        short tally = std::count(C_order.begin(), C_order.end(), i);
        BOOST_CHECK_EQUAL(tally, 1);
    }
    
    Genome D{12};
    std::vector<short> D_order = D.getOrder();
    
    short matchingPointTally = 0;
    for(short i = 0; i < C.getNumVertice(); i++)
    {
        if(C_order[i] == D_order[i])
            matchingPointTally++;
    }
    BOOST_CHECK_NE(matchingPointTally, 0);
    BOOST_CHECK_NE(matchingPointTally, C.getNumVertice());
}

BOOST_AUTO_TEST_CASE(Genome_pathLength)
{
    srand(static_cast<unsigned int>(time(0)));
    rand();
    std::vector<Vertex> points;
    initVetices(points, 3, 12, "/Users/nick/Desktop/test12.txt");
    std::vector<short> order{0,1,2,3,4,5,6,7,8,9,10,11};
    Genome F{12, order};
    F.computePathLength(points);
    BOOST_CHECK_CLOSE_FRACTION(F.getPathLength(), 80.43946838, 0.00005);
}

BOOST_AUTO_TEST_CASE(Genome_mutate_usingAnOrderedGenome)
{
    srand(static_cast<unsigned int>(time(0)));
    rand();
    std::vector<short> order{0,1,2,3,4,5,6,7,8,9};
    Genome G{10, order};
    G.mutate();
    std::vector<short> G_order = G.getOrder();
    short alteredGenes = 0;
    for(short i = 0; i < G.getNumVertice(); i++)
    {
        if(G_order[i] != i)
            alteredGenes++;
    }
    BOOST_CHECK_EQUAL(alteredGenes, 2);
}

BOOST_AUTO_TEST_CASE(crossover_usingOrderGenomes)
{
    srand(static_cast<unsigned int>(time(0)));
    rand();
    std::vector<short> orderOne{0,1,2,3,4,5,6,7,8,9};
    Genome HOne{10, orderOne};
    std::vector<short> orderTwo{8,9,7,6,5,4,3,2,1,0};
    Genome HTwo{10, orderTwo};
    Genome HThree = crossover(HOne, HTwo);
    std::vector<short> HThree_order = HThree.getOrder();
    short alteredGenes = 0;
    for(short i = 0; i < HThree.getNumVertice(); i++)
    {
        if(HThree_order[i] != i)
            alteredGenes++;
    }
    BOOST_CHECK(0 < alteredGenes && alteredGenes < HThree.getNumVertice());
}



BOOST_AUTO_TEST_SUITE_END()
