/*
 * Disjoint.h
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: March 11, 2017
 * Overview: Header for creating a disjoint set of actors/movies
 * Assignment #4
 */

#ifndef DISJOINT_H
#define DISJOINT_H

#include <iostream>
#include <queue>
#include "ActorNode.h"
#include "Movie.h"
#include <map>
#include <queue>

using namespace std;

/* 
 * The Disjoint class creates a node for the disjoint set, used in 
 * actorconnections.cpp
 */
class ActorNode;
class Movie;
class Disjoint {
  public:
    unordered_map<string, Movie*> dMovieMap;
    unordered_map<string, ActorNode*> dActorMap;
    map<int, vector<Movie*>> dYearMap;

    /* Make root of node associated with string1 point to root of node
     * associated with string2
     *
     * node1 (ActorNode*) - the first node to use union on
     * node2 (ActorNode*) - the second node to use union on
     */
    void weightedUnion(ActorNode* node1, ActorNode* node2);

    /* Trace up parent pointers starting at current node until root is 
       reached. Utilizes path compression. Returns the sentinel node of
       the node to find
     * 
     * node (ActorNode*) - the actor node to find
     */
    ActorNode* find(ActorNode* node);

    /* Read from input file to create a forest of actor nodes, stored in
     * dActorMap. Also create and store movies in dMovieMap, and for each movie
     * push the actors who appeared in that movie to its vector of actors.
     * In addition, sort all the years from earliest to latest and store a 
     * vector of movies which appeared in each of those years in dYearMap.
     *
     * in_filename (const char*) - the input file name
     */
    void dLoadFromFile(const char* in_filename);

    /* Destructor */
    ~Disjoint();
};

#endif // DISJOINT_H
