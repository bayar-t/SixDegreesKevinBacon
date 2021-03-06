/*
 * util.cpp
 * Date: March 15, 2017
 * Overview: Contains timer methods. Taken from PA2 starter code
 * Assignment #4
 */

#include <iostream>
#include <sstream>
#include "util.h"

using std::istream;
using std::endl;
using std::cout;
using std::istringstream;
using std::string;
using std::vector;

/*
 * Starts the timer. Saves the current time.
 */
void Timer::begin_timer()
{
    
    start = std::chrono::high_resolution_clock::now();
}

/*
 * Ends the timer. Compares end time with the start time and returns number of nanoseconds
 */
long long Timer::end_timer()
{
    
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    end = std::chrono::high_resolution_clock::now();
    
    return (long long)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

