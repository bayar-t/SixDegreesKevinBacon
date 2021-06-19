/*
 * ActorNode.cpp
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 28, 2017
 * Overview: Implementation to create an actor node. Implementation is inspired
 * by discussions we've had during section on Monday 2/27.
 * Assignment #4
 */

#include "ActorNode.h"

/* Constructor - sets current actor's actorName to input string, actorName 
 *
 * actorName (string) - the input actor name
 */
ActorNode::ActorNode(string actorName) {
  this->actorName = actorName;
  this->done = false;
  this->dist = INT_MAX;
  this->previousActor = NULL;
  this->previousMovie = "";
  this->earliestMovieYear = INT_MAX;
  this->parent = NULL;
  this->treeSize = 1;
}

/*
 * Less-than comparison, allowing ActorNode to work with priority queue
 * Smaller distances have higher priority
 *
 * other (const ActorNode&) - the other Actor node
 */
bool ActorNode::operator<(const ActorNode& other) {
  return dist > other.dist;
}

// Getter for dist
int ActorNode::getDist(){
  return this->dist;
}

// Getter for actor name
string ActorNode::getActorName() {
  return this->actorName;
}

// Getter for previous actor
ActorNode* ActorNode::getPrevActor() {
  return this->previousActor;
}

// Getter for previous movie
string ActorNode::getPrevMovie() {
  return this->previousMovie;
}

// Getter for parent
ActorNode* ActorNode::getParent() {
  return this->parent;
}

// Getter for tree size
int ActorNode::getTreeSize() {
  return this->treeSize;
}

// Getter for movies
vector<Movie*> ActorNode::getMovies() {
  return this->movies;
}

// Getter for done
bool ActorNode::getDone() {
  return this->done;
}

// Getter for earliest movie year
int ActorNode::getEarliestMovieYear() {
  return this->earliestMovieYear;
}

// Setter for distance
void ActorNode::setDist(int dist) {
  this->dist = dist;
}

// Setter for actor name
void ActorNode::setActorName(string actorName) {
  this->actorName = actorName;
}

// Setter for previous actor 
void ActorNode::setPrevActor(ActorNode* actor) {
  this->previousActor = actor;
}

// Setter for previous movie
void ActorNode::setPrevMovie(string prevMovie) {
  this->previousMovie = prevMovie;
}

// Setter for parent
void ActorNode::setParent(ActorNode* parent) {
  this->parent = parent;
}

// Setter for tree size
void ActorNode::setTreeSize(int size) {
  this->treeSize = size;
}

// Setter for movies
void ActorNode::setMovies(vector<Movie*> movies) {
  this->movies = movies;
}

// Setter for done
void ActorNode::setDone(bool done) {
  this->done = done;
}

// Setter for earliest movie year
void ActorNode::setEarliestMovieYear(int year) {
  this->earliestMovieYear = year;
}
