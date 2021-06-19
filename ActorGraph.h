/*
 * ActorGraph.h
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 28, 2017
 * Overview: Header for creating a graph of the actors
 * Assignment #4
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include "ActorNode.h"
#include "Movie.h"

using namespace std;

/* 
 * The ActorGraph class creates a graph where the actors represent the nodes, 
 * and the edges represent movie-relationships between actors
 * */
class ActorNode;
class Movie;
class ActorGraph {
 public: 
    // Maps a string (movieInfo) to a pointer to a movie
    unordered_map<string, Movie*> movieMap;

    // Maps a string (name of actor) to the actorNode associated with that name
    unordered_map<string, ActorNode*> actorMap;

    // Maps an int (year) to a vector of movies released in that year
    map<int, vector<Movie*>> yearMap;

    // Constructor 
    ActorGraph(void);

    /*
     * Load the graph from a tab-delimited file of actor->movie relationships.
     * This function is used in ActorGraph.cpp for pathfinder.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 
     * 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * Return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);

    /*
     * Load the graph from a tab-delimited file of actor->movie relationships.
     * This function is used in ActorGraph.cpp for actorconnections.
     *
     * in_filename - input filename
     *
     * Return true if file was loaded successfully, false otherwise
     */
    bool connectionsLoadFromFile(const char* in_filename);

    /* Reset all member variables of current graph back to their default val */ 
    void resetGraph();

    /* Destructor */
    ~ActorGraph(); 
};

#endif // ACTORGRAPH_H
