/*
 * Network.cpp
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: March 14, 2017
 * Overview: Contains methods to construct a network of Facebook users
 * Assignment #4
 */

#include "Network.h"

/* 
 * Reads the input file and creates all the user nodes, each of which has their
 * list of friends stored in a vector
 *
 * in_filename (const char*) - the input file to read from
 */
bool Network::loadFromFile(const char* in_filename) {
  // Initialize the file stream
  ifstream infile(in_filename);

  // Keep reading lines until the end of file is reached
  while (infile) {
    string buffer;

    // Get the next line
    if (!getline( infile, buffer )) break;

    istringstream string_stream( buffer );
    vector <string> record;

    while (string_stream) {
      string next;

      // Get next string before hitting a space character and put it in 'next'
      if (!getline( string_stream, next, ' ' )) break;
      record.push_back( next );
    }

    if (record.size() != 2) {
      // We should have exactly 2 columns
      cout << "record size is not 2, is instead " << record.size() << endl;
      continue;
    }

    int firstUser = stoi(record[0]);
    int secondUser = stoi(record[1]);

    // Add current user to Facebook network if it's not already in network
    // Then add second user to the first user's friend list
    if(userMap.find(firstUser) == userMap.end()) {
      User* newUser = new User(firstUser);
      vector<int> theFriends = newUser->getFriends();
      theFriends.push_back(secondUser);
      newUser->setFriends(theFriends);
      userMap[firstUser] = newUser;
    }
    else {
      User* user = userMap[firstUser];
      vector<int> theFriends = user->getFriends();
      theFriends.push_back(secondUser);
      user->setFriends(theFriends);
    }

  } // End while loop

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();

  return true;
}

/* Destructor */
Network::~Network() {
  for(auto itr = userMap.begin(); itr != userMap.end(); itr++) {
    delete(itr->second);
  }
}
