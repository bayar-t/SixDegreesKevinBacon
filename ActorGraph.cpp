/*
 * ActorGraph.cpp
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: February 28, 2017
 * Overview: Contains methods to construct a graph of the actors
 * Assignment #4
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format defined in movie_casts.tsv. Feel free to 
 * modify any/all aspects as you wish.
 */
#include "ActorNode.h"
#include "ActorGraph.h"

// Constructor
ActorGraph::ActorGraph(void) {}

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
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
  // Initialize the file stream
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

      // Get next string before hitting a tab character and put it in 'next'
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

    if (use_weighted_edges == false) {
      // Check if current actor already exists in hash map
      // If current actor does not yet exist in hash map, add it to the map
      if(actorMap.find(actor_name) == actorMap.end()) {
        ActorNode* newActor = new ActorNode(actor_name);
        actorMap[actor_name] = newActor;
      }

      // Check if current movie already exists in hash map
      // If current movie already exists in hash map, associate current actor
      // with that movie
      if (movieMap.find(movieInfo) != movieMap.end()) {
        Movie* movie = movieMap[movieInfo];

        vector<ActorNode*> theActors = movie->getActors();
        theActors.push_back(actorMap[actor_name]);
        movie->setActors(theActors);

        vector<Movie*> theMovies = actorMap[actor_name]->getMovies();
        theMovies.push_back(movie);
        actorMap[actor_name]->setMovies(theMovies);
      }

      // If current movie does not yet exist in hash map, create new movie,
      // add it to the map, and associate current actor with that movie
      else {
        Movie* newMovie = new Movie(movieInfo);

        vector<ActorNode*> theActors = newMovie->getActors();
        theActors.push_back(actorMap[actor_name]);
        newMovie->setActors(theActors);
        movieMap[movieInfo] = newMovie;

        vector<Movie*> theMovies = actorMap[actor_name]->getMovies();
        theMovies.push_back(newMovie);
        actorMap[actor_name]->setMovies(theMovies);
      }
    }
    else { // Using weighted edges
      // Check if current actor already exists in hash map
      // If current actor does not yet exist in hash map, add it to the map
      if(actorMap.find(actor_name) == actorMap.end()) {
        ActorNode* newActor = new ActorNode(actor_name);
        actorMap[actor_name] = newActor;
      }

      // Check if current movie already exists in hash map
      // If so, associate current actor with that movie, and assign current
      // movie its appropriate weight
      if (movieMap.find(movieInfo) != movieMap.end()) {
        Movie* movie = movieMap[movieInfo];

        movie->setWeight(1 + (2015-movie_year));

        vector<ActorNode*> theActors = movie->getActors();
        theActors.push_back(actorMap[actor_name]);
        movie->setActors(theActors);
        
        vector<Movie*> theMovies = actorMap[actor_name]->getMovies();
        theMovies.push_back(movie);
        actorMap[actor_name]->setMovies(theMovies);
      }

      // If current movie does not yet exist in hash map, create new movie,
      // add it to the map, and associate current actor with that movie
      else {
        Movie* newMovie = new Movie(movieInfo);

        vector<ActorNode*> theActors = newMovie->getActors();
        theActors.push_back(actorMap[actor_name]);
        newMovie->setActors(theActors);
        movieMap[movieInfo] = newMovie;

        vector<Movie*> theMovies = actorMap[actor_name]->getMovies();
        theMovies.push_back(newMovie);
        actorMap[actor_name]->setMovies(theMovies);
      }
    }

  } // End while loop


  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  return true;
}

/*
 * Load the graph from a tab-delimited file of actor->movie relationships.
 * This function is used in ActorGraph.cpp for actorconnections.
 *
 * in_filename - input filename
 *
 * Return true if file was loaded successfully, false otherwise
 */
bool ActorGraph::connectionsLoadFromFile(const char* in_filename) {
  // Initialize the file stream
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

    // Check if current movie already exists in hash map
    // If current movie does not yet exist in map, create a new movieless
    // actor and add it to actorMap
    if(actorMap.find(actor_name) == actorMap.end()) {
      ActorNode* newActor = new ActorNode(actor_name);
      actorMap[actor_name] = newActor;
    }

    // Check if current movie already exists in hash map
    // If so, associate current actor with that movie, and assign current
    // movie its appropriate weight
    // Also associate current movie with the current year via yearMap
    if (movieMap.find(movieInfo) != movieMap.end()) {
      Movie* movie = movieMap[movieInfo];

      vector<ActorNode*> theActors = movie->getActors();
      theActors.push_back(actorMap[actor_name]);
      movie->setActors(theActors);

      if (movie_year < actorMap[actor_name]->getEarliestMovieYear()) {
        actorMap[actor_name]->setEarliestMovieYear(movie_year);
      }

      yearMap[movie_year].push_back(movie);
    }
    // If current movie does not yet exist in hash map, create new movie,
    // add it to the map, and associate current actor with that movie
    // Also associate current movie with the current year via yearMap
    else {
      Movie* newMovie = new Movie(movieInfo); 

      vector<ActorNode*> theActors = newMovie->getActors();
      theActors.push_back(actorMap[actor_name]);
      newMovie->setActors(theActors);
      movieMap[movieInfo] = newMovie;
      if (movie_year < actorMap[actor_name]->getEarliestMovieYear()) {
        actorMap[actor_name]->setEarliestMovieYear(movie_year);
      }
      yearMap[movie_year].push_back(newMovie);
    }

  }
}

// Reset all member variables of current graph back to their default values
void ActorGraph::resetGraph(){
  // Iterate through all actors and reset their member variables
  for (auto actIt = this->actorMap.begin(); actIt != this->actorMap.end(); 
      actIt++){
    actIt->second->setDist(INT_MAX);
    actIt->second->setPrevActor(NULL);
    actIt->second->setPrevMovie("");
    actIt->second->setDone(false);
  }
}

// Destructor for ActorGraph
ActorGraph::~ActorGraph() {
  // Delete all movies
  for (auto it = this->movieMap.begin(); it != this->movieMap.end(); it++) {
    delete(it->second);
  }
  // Delete all actors
  for (auto actItr = this->actorMap.begin(); actItr != this->actorMap.end(); 
      actItr++) {
    delete(actItr->second);
  }
}


