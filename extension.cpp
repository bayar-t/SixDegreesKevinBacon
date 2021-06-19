/*
 * extension.cpp
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: March 14, 2017
 * Overview: Uses the greedy algorithm to find out how many Facebook users
 * need to post the same message on Facebook for everyone on Facebook to
 * see that message
 * Assignment #4
 */

#include <iostream>
#include "Network.h"

/* This program finds out the approximate minimum number of users needed to
 * post the same message on a social network and have all the users in the
 * network to receive the message
 */
int main(int argc, char**argv) {

  if(argc != 3) {
    cout << "Incorrect number of arguments, expected 3, got: " << argc << endl;
  }

  // Add all users to Facebook network
  // For each user, add all user's friends to his/her friend list
  Network* network = new Network();
  network->loadFromFile(argv[1]);

  unordered_map<int, User*> users = network->userMap;

  ofstream outFile(argv[2]);
  vector<User*> dominantUsers;

  int largestFriendSize;
  User* largestUser;
  int numLeftToCover = users.size();
  unordered_map<int, User*> uncoveredUsers;
  // Keep finding users until all users have been covered (received message)
  while (numLeftToCover > 0) {
    largestFriendSize = 0;

    // Store all the uncovered users in an unordered map for fast retrieval
    for (auto userItr = users.begin(); userItr != users.end(); userItr++) {
      User* currUser = userItr->second;
      if (currUser->getCovered() == false) {
        uncoveredUsers[userItr->first] = userItr->second;
      }
    }
    // Iterate through all users
    for (auto userItr = uncoveredUsers.begin(); userItr != uncoveredUsers.end(); 
         userItr++) {
      // Get current user's friend list size
      User* currUser = userItr->second;
      vector<int> currUserFriendList = currUser->getFriends();
      int currUserFriendSize = currUserFriendList.size();

      // Find the user with the most uncovered friends
      if (currUserFriendSize >= largestFriendSize) {
        largestFriendSize = currUserFriendSize;
        largestUser = currUser;
      }
    }
    
    // Set the user with the most uncovered friends to cover and push this
    // user to the dominant set.
    largestUser->setCovered(true);
    numLeftToCover--;
    dominantUsers.push_back(largestUser);
    
    // Set all the uncovered friends of this user to covered
    for (int index = 0; index < largestUser->getFriends().size(); index++) {

      vector<int> theFriends = largestUser->getFriends(); 
      network->userMap[theFriends[index]]->setCovered(true);
      numLeftToCover--;
    }
   
    // Remove the covered users from every users friends vector
    for (auto userItr = users.begin(); userItr != users.end(); userItr++) {
      User* checkUser = userItr->second;
      if (checkUser->getCovered()) {
        for (auto remItr = users.begin(); remItr != users.end(); remItr++) {
          User* currUser = remItr->second;
          for (int index = 0; index < currUser->getFriends().size(); index++) {
            vector<int> theFriends = currUser->getFriends();
            if (checkUser->getUserID() == theFriends[index]) {
              theFriends.erase(theFriends.begin() + index);
              currUser->setFriends(theFriends);
            }
          }
        }
      }
    }

    // Reset the uncovered users map
    uncoveredUsers.clear();
  }

  outFile << "In order for all users to be influenced, the network may need ";
  outFile << dominantUsers.size() << " different\nusers to post the message.";
  outFile << "\nThe users associated with the following IDs can post the ";
  outFile << "message\nfor the message to reach everyone in the network:\n\n";

  for (int index = 0; index < dominantUsers.size(); index++) {
    outFile << dominantUsers[index]->getUserID() << " ";
    // Append new line after every 15 users, for style purposes
    if (index != 0 && index % 15 == 0) {
      outFile << endl;
    }
  }

  outFile << endl;
  delete(network);
  return 0;
}
