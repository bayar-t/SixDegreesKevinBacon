/*
 * Movie.cpp
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 28, 2017
 * Overview: Creates a movie node
 * Assignment #4
 */

#include "Movie.h"

// Getter for actors
vector<ActorNode*> Movie::getActors() {
  return this->actors;
}

// Getter for movie name
string Movie::getMovieName() {
  return this->movieName;
}

// Getter for movie year
string Movie::getMovieYear() {
  return this->movieYear;
}

// Getter for movie info
string Movie::getMovieInfo() {
  return this->movieInfo;
}

// Getter for weight
int Movie::getWeight() {
  return this->weight;
}

// Setter for actors
void Movie::setActors(vector<ActorNode*> actors){
  this->actors = actors;
}

// Setter for movie name
void Movie::setMovieName(string movieName) {
  this->movieName = movieName;
}

// Setter for movie year
void Movie::setMovieYear(string movieYear) {
  this->movieYear = movieYear;
}

// Setter for movie info
void Movie::setMovieInfo(string movieInfo) {
  this->movieInfo = movieInfo;
}

// Setter for weight
void Movie::setWeight(int weight) {
  this->weight = weight;
}
