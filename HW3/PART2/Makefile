
#Use GNU compiler
cc = g++
ccFLAGS= -g -std=c++11
WARNFLAGS= -Wall -Wextra -pedantic
Library = -lpthread

objects = dinningPhilosopherMonitor.o main.o

output = main

all: $(output)

$(output): $(objects)
	$(cc) $(ccFLAGS) $(WARNFLAGS) $(objects) $(Library) -o $@

dinningPhilosopherMonitor.o : dinningPhilosopherMonitor.cpp dinningPhilosopherMonitor.hpp
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c dinningPhilosopherMonitor.cpp

main.o : main.cpp
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c main.cpp

.PHONY : clean

clean :
	rm -f $(objects) $(output)

