/*
 * Network.h
 * Authors: Jamie Shi (A12678538) and Sergelenbayar Tsogtbaatar (A12949154)
 * Date: March 14, 2017
 * Overview: Header for Network.h, which constructs the network of Facebook
 * users
 * Assignment #4
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "User.h"
#include <string>

using namespace std;

/*
 * Header for Network.h, which constructors the network of Facebook
 */
class User;
class Network {

  public:
    unordered_map<int, User*> userMap;

    /*
     * Load the graph from a tab-delimited file of user->user relationships.
     *
     * in_filename - input filename
     */
    bool loadFromFile(const char* in_filename);

    /* Destructor */
    ~Network();

};

#endif // NETWORK_H
