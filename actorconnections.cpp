/*
 * actorconnections.cpp
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: March 1, 2017
 * Overview: Finds the earliest path from one actor to another actor
 * Assignment #4
 */

#include "ActorGraph.h"
#include "Disjoint.h"
#include "util.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <climits>
#include <sstream>
#include <string.h>
#include <string>
#include <algorithm>

using namespace std;

/*
 * Use breadth first search to check if a path exists between the current actor
 * (actor in startNode) and the actor to find. Based on lecture 18, pg 31 slide
 *
 * theGraph (ActorGraph*) - the graph to use BFS on
 * startNode (ActorNode*) - the source node
 * actorToFind (string) - the actor to find
 *
 * Returns a vector of ActorNodes, representing the shortest path from the
 * startNode's actor to the actor to find
 */
bool bfs(ActorGraph* theGraph, ActorNode* startNode, string actorToFind) {
  queue<ActorNode*> toExplore;
  toExplore.push(startNode);
  startNode->setDist(0);

  // Keep searching until actor is found
  while (!toExplore.empty()) {
    ActorNode* next = toExplore.front();
    toExplore.pop();

    // If we end node (actor) is found, push the path of nodes in order from
    // end node to start node to the shortestPath vector
    if (next->getActorName() == actorToFind) {
      return true;
    }

    // Traverse all movies this actor appeared in
    vector<Movie*> movieVec = next->getMovies();
    for (int movieIndex = 0; movieIndex < movieVec.size(); movieIndex++) {
      vector<ActorNode*> actorVec = movieVec[movieIndex]->getActors();

      // Traverse all actors that appeared in current movie
      for (int actIndex = 0; actIndex<actorVec.size(); actIndex++) {
        ActorNode* actorNeighbor = actorVec[actIndex];
        Movie* neighborMovie = movieVec[movieIndex];

        // Push actor to queue if it hasn't been visited yet
        if (next->getDist()+1 < actorNeighbor->getDist()) {
          actorNeighbor->setDist(next->getDist()+1);
          actorNeighbor->setPrevActor(next);
          actorNeighbor->setPrevMovie(neighborMovie->getMovieInfo());
          toExplore.push(actorNeighbor);
        }
      }
    }
  }

  return false;
}


/* 
 * Prints the earliest year in which two actors were connected by a path.
 * The algorithm used to find the path depends on the user's preference
 */
int main(int argc, char** argv){

  int latestYear = 0;
  bool useBFS = false;
  bool useWeightedEdges;
  Timer t;

  // If only 4 input arguments, use ufind by default
  if (argc == 4) {
    useBFS = false;
  }
  else {
    if (argc != 5) {
      cout << "Invalid number of arguments. Expected 5, got " << argc << endl;
      return -1;
    }
    if (strcmp(argv[4], "bfs") == 0) {
      useBFS = true;
    }
  }

  ActorGraph* graph;
  Disjoint* disjoint;
  vector<pair<string, string>> pairs;
  if (useBFS) {
    // If using BFS, create the graph
    graph = new ActorGraph();
    graph->connectionsLoadFromFile(argv[1]);
  }
  else {
    disjoint = new Disjoint();
    disjoint->dLoadFromFile(argv[1]);
  }

  ifstream infile(argv[2]);
  ofstream outFile(argv[3]);
  outFile << ("Actor1\tActor2\tYear\n");
  bool have_header = false;
  bool hasConnection = false;
  // Keep reading lines until the end of file is reached

  t.begin_timer();
  while (infile) {
    hasConnection = false;
    string buffer;
    // Get the next line
    if (!getline(infile, buffer)) break;

    if (!have_header) {
      // Skip the header
      have_header = true;
      continue;
    }

    istringstream string_stream(buffer);
    vector <string> record;

    while (string_stream) {
      string next;

      // Get the next string before hitting a tab character and put it in 'next'
      if (!getline( string_stream, next, '\t' )) break;
      record.push_back( next );
    }

    if (record.size() != 2) {
      // We should have exactly 2 columns
      cout << "record size is not 2, is instead " << record.size() << endl;
      continue;
    }

    string startActor = record[0];
    string actorToFind = record[1];
    pairs.push_back(make_pair(startActor, actorToFind));

  } // End while

  if (useBFS) {
    vector<int> years (pairs.size(), 9999);

    auto yearMap = graph->yearMap;
    int currentYear = 0;
    bool connectionExists = false;

    // Iterate through all the years that had movies
    for (auto yearItr = graph->yearMap.begin(); yearItr != graph->yearMap.end(); 
         yearItr++) {

      vector<Movie*> movieVector = yearItr->second;

      // Iterate through all the movies released in that year
      for (int movieIndex = 0; movieIndex < movieVector.size(); movieIndex++) {

        vector<ActorNode*> currMovieActors = movieVector[movieIndex]->getActors();
        // Iterate through all the actors that appeared in this movie
        // Add edges to these actors (make them associated with current movie)
        for (int actIndex = 0; actIndex < currMovieActors.size(); actIndex++) {
          string actName = currMovieActors[actIndex]->getActorName();
          vector<Movie*> actorMovies = graph->actorMap[actName]->getMovies();

          // Don't push duplicate movies to same actor's list of movies
          if (find(actorMovies.begin(), actorMovies.end(), 
              movieVector[movieIndex]) == actorMovies.end()) {
            vector<Movie*> theMovies = graph->actorMap[actName]->getMovies();
            theMovies.push_back(movieVector[movieIndex]);
            graph->actorMap[actName]->setMovies(theMovies);
          }
        }
      }

      // Iterate through all the pairs of actors; see if they're connected
      for (int pairIndex = 0; pairIndex < pairs.size(); pairIndex++) {
        
        // If years[pairIndex] != 9999, this means that an earlier connection
        // between the pair of actors has already been found, so you don't need
        // to perform BFS again
        if (years[pairIndex] != 9999) {
          continue;
        }

        // Every time we rerun BFS, reset all actor nodes' distances
        for (auto actItr = graph->actorMap.begin(); 
             actItr != graph->actorMap.end(); actItr++) {
          // Edge case: if actor is null, prevent seg fault by not accessing
          // its dist member variable
          if (!actItr->second) {
            continue;
          } 
          actItr->second->setDist(INT_MAX);
        }

        ActorNode* startActor = graph->actorMap[pairs[pairIndex].first];
        ActorNode* actorToFind = graph->actorMap[pairs[pairIndex].second];
        int earliest = 0;

        // Edge case: prevent potential seg fault by not accessing member
        // variables of a null actor
        if (!startActor || !actorToFind) {
          continue;
        }

        // "earliest" is the earliest year in which one of the two actors in
        // the pair appeared in a movie
        if (startActor->getEarliestMovieYear() <= actorToFind->getEarliestMovieYear()) {
          earliest = startActor->getEarliestMovieYear();
        }
        else {
          earliest = actorToFind->getEarliestMovieYear();
        }

        // Optimization: don't use bfs until both actors at least have one
        // movie
        int currYear = yearItr->first; 
        bool connectionExists = false;
        if (currYear >= earliest) {
          connectionExists = bfs(graph, startActor, actorToFind->getActorName());
        }
        if (connectionExists) {
          // Set the earliest year the pair of actors are connected
          if (years[pairIndex] == 9999) {
            years[pairIndex] = currYear;
          }
        }

      }

    }

    // Output results (earliest year the actor pair was connected) to output 
    // file
    for (int index = 0; index < pairs.size(); index++) {
      outFile << pairs[index].first << "\t" << pairs[index].second << "\t" << years[index] << endl;
    }

  }

  if (!useBFS) {
    vector<int> years (pairs.size(), 9999);
    auto yearMap = disjoint->dYearMap;
    int currentYear = 0;
    bool connectionExists = false;

    // Iterate through all the years that had movies
    for (auto yearItr = yearMap.begin(); yearItr != yearMap.end(); yearItr++) {

      vector<Movie*> movies = yearItr->second;

      // Iterate through all the movies released in that year
      for (int movieIndex = 0; movieIndex < movies.size(); movieIndex++) {
        vector<ActorNode*> actors = movies[movieIndex]->getActors();
        for (int actIndex = 0; actIndex < actors.size()-1; actIndex++){
          disjoint->weightedUnion(actors[actIndex], actors[actIndex+1]);
        }
      }

      // Iterate through all actor pairs and if found, set years
      for(int pairIndex = 0; pairIndex < pairs.size(); pairIndex++) {
        ActorNode* firstActor = disjoint->dActorMap[pairs[pairIndex].first];
        ActorNode* firstFound = disjoint->find(firstActor);
        ActorNode* secondActor = disjoint->dActorMap[pairs[pairIndex].second];
        ActorNode* secondFound = disjoint->find(secondActor);
        if (firstFound == secondFound) {
          if (years[pairIndex] == 9999) {
            years[pairIndex] = yearItr->first;
            if(firstActor == secondActor){
              years[pairIndex] = firstActor->getEarliestMovieYear();
            }
          }
        }
      }
    }

    for (int index = 0; index < pairs.size(); index++) {
      outFile << pairs[index].first << "\t" << pairs[index].second << "\t";
      outFile << years[index] << endl;
    }
  } // End if

  // Clear memory
  if (useBFS) {
    delete(graph);
  }
  else {
    delete(disjoint);
  }

  long long time = t.end_timer();
  time = time / 1000000;
  cout << "The runtime in milliseconds is: " << time << endl;

  return 0;
}
