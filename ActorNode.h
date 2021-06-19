/*
 * ActorNode.h
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 28, 2017
 * Overview: Header for creating an actor node. Implementation is inspired by
 * discussions we've had during section on Monday 2/27.
 * Assignment #4
 */

#ifndef ACTORNODE_H
#define ACTORNODE_H

#define __STDC_LIMIT_MACROS

#include <iostream>
#include <limits>
#include <climits>
#include <stdint.h>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

/* The ActorNode class creates an actor node, which contains information about 
 * the current actor and movies that this actor has took part in */
class Movie;
class ActorNode {

  private:
    string actorName;
    int dist;
    ActorNode* previousActor;
    string previousMovie;
    ActorNode* parent;
    int treeSize;
    vector<Movie*> movies;
    bool done;
    int earliestMovieYear;

  public:
    /*
     * Less-than comparison, allowing ActorNode to work with priority queue
     * Smaller distances have higher priority
     *
     * other (const ActorNode&) - the other Actor node
     */
    bool operator<(const ActorNode& other);

    /* Constructor - sets current actor's actorName to input string, actorName 
     *
     * actorName (string) - the input actor name
     */
    ActorNode(string actorName);

    // Getters
    int getDist();
    string getActorName();
    ActorNode* getPrevActor();
    string getPrevMovie();
    ActorNode* getParent();
    int getTreeSize();
    vector<Movie*> getMovies();
    bool getDone();
    int getEarliestMovieYear();

    // Setters
    void setDist(int dist);
    void setActorName(string actorName);
    void setPrevActor(ActorNode* actor);
    void setPrevMovie(string prevMovie);
    void setParent(ActorNode* parent);
    void setTreeSize(int size);
    void setMovies(vector<Movie*> movies);
    void setDone(bool done);
    void setEarliestMovieYear(int year);
};

#endif // ACTORNODE_H
