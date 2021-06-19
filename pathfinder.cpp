/*
 * pathfinder.cpp
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: March 1, 2017
 * Overview: Finds the shortest path from one actor to another actor, via
 * movies that they both appeared in
 * Assignment #4
 */

#include "ActorGraph.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <climits>
#include <sstream>
#include <string>
#include <string.h>
using namespace std;

/* 
 * Comparator class for custom priority queue, used in Dijikstra's algorithm,
 * converting default max heap implementation to that of min heap
 */
class ActorNodePtrComp {
  public:
    bool operator()(ActorNode*& lhs, ActorNode*& rhs) const {
      return *lhs < *rhs;
    }
};

/*
 * Use BFS on the graph to see if there exists a connection between the input
 * start node's actor and the input actorToFind. If a connection exists, 
 * return a vector of ActorNodes, representing the shortest path from the 
 * start actor to the end actor in reverse order. If a connection does not 
 * exist, return an empty vector.
 *
 * theGraph (ActorGraph*) - the graph to use BFS on
 * startNode (ActorNode*) - start node, containing start actor to search from
 * actorToFind (string) - the actor to find in the graph
 *
 * Implementation is based on lecture 18 slide on page 31
 */
vector<ActorNode*> bfs(ActorGraph* theGraph, ActorNode* startNode, 
                       string actorToFind) {

  vector<ActorNode*> emptyVec;

  // Edge case: start node is null
  if (!startNode) {
    return emptyVec;
  }

  queue<ActorNode*> toExplore;
  toExplore.push(startNode);
  startNode->setDist(0);

  // Keep doing BFS until queue is empty or actor is found
  while (!toExplore.empty()) {
    ActorNode* currActor = toExplore.front();
    toExplore.pop();

    // If the end node (actor) is found, push the path of nodes in order from
    // end node to start node to the shortestPath vector
    if (currActor->getActorName() == actorToFind) {
      vector<ActorNode*> shortestPath;
      while (currActor->getPrevActor()) {
        shortestPath.push_back(currActor);
        currActor = currActor->getPrevActor();
      }
      return shortestPath;
    }

    // Iterate through all the movies
    vector<Movie*> movieVec = currActor->getMovies();
    for (int movieIndex = 0; movieIndex < movieVec.size(); movieIndex++) {
      // Iterate through all the actors associated with current movie
      vector<ActorNode*> actorVec = movieVec[movieIndex]->getActors();
      for (int actIndex = 0; actIndex<actorVec.size(); actIndex++) {
        ActorNode* actorNeighbor = actorVec[actIndex];
        Movie* neighborMovie = movieVec[movieIndex];
        // If actor is not yet visited, set neighbor's dist to current dist+1
        // and add to queue for further traversal
        if (currActor->getDist()+1 < actorNeighbor->getDist()) {
          actorNeighbor->setDist(currActor->getDist()+1);
          actorNeighbor->setPrevActor(currActor);
          actorNeighbor->setPrevMovie(neighborMovie->getMovieInfo());
          toExplore.push(actorNeighbor);
        }
      }
    }
  }

  // If actor connection is not found, return empty vector
  return emptyVec;
}

/* 
 * Use dijkstra's algorithm on current graph to find the path with the lowest
 * weight. Return a vector of ActorNodes, representing the path with the lowest
 * weight in reverse order.
 *
 * graph (ActorGraph*) - the graph to use dijkstra's algorithm on
 * startNode (ActorNode*) - start node containing actor to start from
 * actorToFind (string) - the actor to search for in the graph
 *
 * Implementation based on Stepik 4.5 (Dijkstra's Algorithm) reading assignment
 */
vector<ActorNode*> dijkstra(ActorGraph* graph, ActorNode* startNode, 
                            string actorToFind){
  // Custom priority queue using min heap implementation
  priority_queue<ActorNode*, vector<ActorNode*>, ActorNodePtrComp> actorQueue;

  // Reset distances of all actors back to INT_MAX
  for(auto itr = graph->movieMap.begin(); itr != graph->movieMap.end(); itr++){
    vector<ActorNode*> actors = itr->second->getActors();
    for(int index = 0; index < actors.size(); index++){
      actors[index]->setDist(INT_MAX);
    }
  }

  startNode->setDist(0);
  actorQueue.push(startNode);
  // Keep using dijkstra's algorithm until priority queue is empty or the
  // actorToFind is found
  while (!actorQueue.empty()) {
    ActorNode* topActor = actorQueue.top();
    actorQueue.pop();

    // If actorToFind is found, traverse backwards and save path to pathVector
    if (topActor->getActorName() == actorToFind) {
      ActorNode* found = topActor;
      vector<ActorNode*> pathVector;
      // Keep traversing backwards (i.e. get current actor's previous actor)
      while (found->getPrevActor()) {
        pathVector.push_back(found);
        found = found->getPrevActor();
      }
      return pathVector;
    }

    // If actorToFind is not yet found, then visit it
    if (!topActor->getDone()) {
      topActor->setDone(true);
      vector<Movie*> movieVec = topActor->getMovies();
      // Iterate through all movies
      for(int movieIndex = 0; movieIndex < movieVec.size(); movieIndex++) {
        vector<ActorNode*> actorVec = movieVec[movieIndex]->getActors();
        // Iterate through all actors associated with current movie
        for(int actIndex = 0; actIndex < actorVec.size(); actIndex++) {
          // If current actor has not yet been visited, update its weight 
          // and dist
          if (!actorVec[actIndex]->getDone()) {
            int weight = topActor->getDist() + movieVec[movieIndex]->getWeight();
            ActorNode* currentActor = actorVec[actIndex];
            // Update current actor's member variables only if its weight is
            // the smallest so far
            if (weight < actorVec[actIndex]->getDist()) {
              currentActor->setPrevMovie(movieVec[movieIndex]->getMovieInfo());
              currentActor->setPrevActor(topActor);
              currentActor->setDist(weight);
              actorQueue.push(currentActor);
            }
          }
        }
      }
    }
  } // End while
}

/* 
 * Prints the shortest path or path with the lowest weight (depending on the
 * parameters) to the outFile.
 */
int main(int argc, char** argv) {
  if (argc != 5) {
    cout << "Invalid number of arguments. Expected 5, got " << argc << endl;
    return -1;
  }

  bool useWeightedEdges;
  if (strcmp(argv[2], "u") == 0) {
    useWeightedEdges = false;
  }
  else {
    useWeightedEdges = true;
  }

  // Create the graph
  ActorGraph* graph = new ActorGraph();
  graph->loadFromFile(argv[1], useWeightedEdges);
  ifstream infile(argv[3]);
  ofstream outFile(argv[4]);
  outFile << ("(actor)--[movie#@year]-->(actor)--...\n");

  bool have_header = false;
  // Keep reading lines until the end of file is reached
  while (infile) {
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

      // Get the next string before hitting a tab character and put it in next
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
    ActorNode* startNode = graph->actorMap[startActor];

    vector<ActorNode*> prevActors;
    vector<ActorNode*> realActors;

    // Use dijkstra's algorithm to find path if user specified using weighted
    // edges
    if (useWeightedEdges == true) {
      graph->resetGraph();
      prevActors = dijkstra(graph, startNode, actorToFind);
    }
    // Use BFS to find path otherwise
    else {
      graph->resetGraph();
      prevActors = bfs(graph, startNode, actorToFind);
    }

    // Output the path to the outFile
    outFile << "(" + startActor + ")--";
    for (int i=prevActors.size()-1; i>0; i--) {
      outFile << "[" << prevActors.at(i)->getPrevMovie() <<  "]-->";
      outFile << "(" << prevActors[i]->getActorName() << ")--";
    }

    outFile << "[" << prevActors.at(0)->getPrevMovie() << "]-->";
    outFile << "(" << prevActors[0]->getActorName() << ")";
    outFile << endl;
  }

  // Free memory after outputting path to outFile
  delete(graph);

  return 0;

}
