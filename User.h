/*
 * User.h
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: March 14, 2017
 * Overview: Header for user.cpp, which creates a Facebook user
 * Assignment #4
 */

#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>

using namespace std;

/* Used for creating a Facebook user with user's information*/
class User {
  private:
    int userID;
    bool covered; // True if user has received the message
    vector<int> friends;

  public:
    /*
     * Constructor to create a Facebook user
     *
     * userID (int) - the input user ID
     */
    User(int userID);

    // Getters
    int getUserID();
    bool getCovered();
    vector<int> getFriends();

    // Setters
    void setUserID(int userID);
    void setCovered(bool covered);
    void setFriends(vector<int> friends);

};

#endif // USER_H
