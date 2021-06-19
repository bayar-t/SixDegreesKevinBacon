# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
    CPPFLAGS += -O3
    LDFLAGS += -O3
else
    CPPFLAGS += -g
    LDFLAGS += -g
endif

#all: pathfinder
all: actorconnections pathfinder extension

# include what ever source code *.h files pathfinder relies on (these are merely the ones that were used in the solution)

actorconnections: ActorGraph.o ActorNode.o Movie.o Disjoint.o util.o
pathfinder: ActorGraph.o ActorNode.o Movie.o 
extension: User.o Network.o

# include what ever source code *.h files ActorGraph relies on (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.h file that is already included with class/method headers

ActorGraph.o: ActorNode.o Movie.o ActorGraph.h
ActorNode.o: ActorNode.h
Movie.o: Movie.h
Disjoint.o: Disjoint.h
User.o: User.h
Network.o: User.o Network.h
util.o: util.h

clean:
	rm -f pathfinder *.o core*

