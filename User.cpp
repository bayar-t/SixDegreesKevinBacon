/*
 * User.cpp
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: March 14, 2017
 * Overview: Creates a Facebook user containing the user's information
 * Assignment #4
 */

#include "User.h"

/*
 * Constructor to create a Facebook user
 *
 * userID (int) - the current user's user ID
 */
User::User(int userID) {
  this->userID = userID;
  this->covered = false;
}
// Getter for userID
int User::getUserID() {
  return this->userID;
}
// Getter for covered
bool User::getCovered() {
  return this->covered;
}
// Getter for friends
vector<int> User::getFriends() {
  return this->friends;
}

// Setter for userID
void User::setUserID(int userID) {
  this->userID = userID;
}
// Setter for covered
void User::setCovered(bool covered) {
  this->covered = covered;
}
// Setter for friends
void User::setFriends(vector<int> friends) {
  this->friends = friends;
}


