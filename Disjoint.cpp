/*
 * Disjoint.cpp
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: March 11, 2017
 * Overview: Creates a disjoint set of actors/movies
 * Assignment #4
 */

#include "Disjoint.h"
#include <fstream>
#include <sstream>

/* Make root of node associated with string1 point to root of node
 * associated with string2
 *
 * node1 (ActorNode*) - the first node to use union on
 * node2 (ActorNode*) - the second node to use union on
 */
void Disjoint::weightedUnion(ActorNode* node1, ActorNode* node2) {
  // Edge case: inputs cannot be null
  if (!node1 || !node2) {
    return;
  }

  ActorNode* node1Root = find(node1);
  ActorNode* node2Root = find(node2);

  // If size of tree containing node1 is smaller than that of the tree 
  // containing node2, make root node1's parent point to node2's root
  // and update the size accordingly
  if (node1Root->getTreeSize() < node2Root->getTreeSize()) {
    node1Root->setParent(node2Root);
    node2Root->setTreeSize(node1Root->getTreeSize() + node2Root->getTreeSize()); 
  }
  else {
    node2Root->setParent(node1Root);
    node1Root->setTreeSize(node1Root->getTreeSize() + node2Root->getTreeSize());
  }
}

/* Trace up parent pointers starting at current node until root is 
   reached. Utilizes path compression. Returns the sentinel node of
   the node to find
 * 
 * node (ActorNode*) - the actor node to find
 */
ActorNode* Disjoint::find(ActorNode* node) {
  // Edge case: input cannot be null
  if (!node) {
    return NULL;
  }

  queue<ActorNode*> nodesQueue;
  ActorNode* root = node;

  // Get the sentinel
  while (root->getParent() != root) {
    nodesQueue.push(root);
    root = root->getParent();
  }

  // Set all nodes' parent in queue to sentinel
  while (!nodesQueue.empty()) {
    ActorNode* topNode = nodesQueue.front();
    nodesQueue.pop();
    topNode->setParent(root);
  }

  return root;
}

/* Read from input file to create a forest of actor nodes, stored in
 * dActorMap. Also create and store movies in dMovieMap, and for each
 * movie push the actors who appeared in that movie to its vector of actors.
 * In addition, sort all the years from earliest to latest and store a 
 * vector of movies which appeared in each of those years in dYearMap.
 *
 * in_filename (const char*) - the input file name
 */
void Disjoint::dLoadFromFile(const char* in_filename){
  ifstream infile(in_filename);

  bool have_header = false;
  // Keep reading lines until the end of file is reached
  while (infile) {
    string buffer;

    // Get the next line
    if (!getline( infile, buffer )) break;

    if (!have_header) {
      // Skip the header
      have_header = true;
      continue;
    }

    istringstream string_stream( buffer );
    vector <string> record;

    while (string_stream) {
      string next;

      // Get the next string before hitting a tab character and put it in 'next'
      if (!getline( string_stream, next, '\t' )) break;

      record.push_back( next );
    }

    if (record.size() != 3) {
      // We should have exactly 3 columns
      cout << "record size is not 3, is instead " << record.size() << endl;
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);
    string stringYear(record[2]);
    string movieInfo = movie_title +"#@" + stringYear;

    // Add each unique actor to actorMap to create a forest of movieless actors
    if(dActorMap.find(actor_name) == dActorMap.end()) {
      ActorNode* newActor = new ActorNode(actor_name);
      newActor->setParent(newActor);
      dActorMap[actor_name] = newActor;
    }

    // Check if current movie already exists in hash map
    // If so, associate current actor with that movie, and assign current
    // movie its appropriate weight
    // Also associate current movie with the current year via yearMap
    if (dMovieMap.find(movieInfo) != dMovieMap.end()) {
      Movie* movie = dMovieMap[movieInfo];
      vector<ActorNode*> theActors = movie->getActors();
      theActors.push_back(dActorMap[actor_name]);
      movie->setActors(theActors);
      // Set actor's earliestMovieYear to the earliest year it starred in a
      // movie
      if (movie_year < dActorMap[actor_name]->getEarliestMovieYear()) {
        dActorMap[actor_name]->setEarliestMovieYear(movie_year);
      }
      dYearMap[movie_year].push_back(movie);
    }
    // If current movie does not yet exist in hash map, create new movie,
    // add it to the map, and associate current actor with that movie
    // Also associate current movie with the current year via yearMap
    else {
      Movie* newMovie = new Movie(movieInfo);
      vector<ActorNode*> theActors = newMovie->getActors();
      theActors.push_back(dActorMap[actor_name]);
      newMovie->setActors(theActors);
      dMovieMap[movieInfo] = newMovie;
      // Set actor's earliestMovieYear to the earliest year it starred in a
      // movie
      if (movie_year < dActorMap[actor_name]->getEarliestMovieYear()) {
        dActorMap[actor_name]->setEarliestMovieYear(movie_year);
      }
      dYearMap[movie_year].push_back(newMovie);
    }

  }

}

/* Destructor */
Disjoint::~Disjoint() {
  // Delete all movies
  for(auto it = this->dMovieMap.begin(); it != this->dMovieMap.end(); it++){
    delete(it->second);
  }
  // Delete all actors
  for(auto actItr = this->dActorMap.begin(); actItr != this->dActorMap.end(); actItr++){
    delete(actItr->second);
  }
}
