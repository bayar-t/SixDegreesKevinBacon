/*
 * Movie.h
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 28, 2017
 * Overview: Header for creating a movie node
 * Assignment #4
 */

#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

/* The Movie class creates a movie node, which contains information about the 
 * current movie and actors that were in this movie */
class ActorNode;
class Movie {

  private:
    vector<ActorNode*> actors;
    string movieName;
    string movieYear;
    string movieInfo; // movieInfo = movieName + movieYear
    int weight = 0;

  public:
    // Constructor - initialize movieInfo to the input parameter movieInfo
    Movie(string movieInfo)
      : movieInfo(movieInfo) {};

    // Getters
    vector<ActorNode*> getActors();
    string getMovieName();
    string getMovieYear();
    string getMovieInfo();
    int getWeight();

    // Setters
    void setActors(vector<ActorNode*> actors);
    void setMovieName(string movieName);
    void setMovieYear(string movieYear);
    void setMovieInfo(string movieInfo);
    void setWeight(int weight);
};

#endif // MOVIE_H


