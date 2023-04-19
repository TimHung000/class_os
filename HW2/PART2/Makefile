
#Use GNU compiler
cc = gcc
ccFLAGS= -g -std=c99
CC = g++
WARNFLAGS= -Wall -Wextra -pedantic
Library = -lpthread

objects = dinningPhilosopher.o main.o

output = dinningPhilosopher


all: $(output)

$(output): $(objects)
	$(cc) $(ccFLAGS) $(WARNFLAGS) $(objects) $(Library) -o $@

dinningPhilosopher.o : dinningPhilosopher.c dinningPhilosopher.h
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c dinningPhilosopher.c

main.o : main.c
	$(cc) $(ccFLAGS) $(WARNFLAGS) -c main.c

.PHONY : clean

clean :
	rm -f $(objects) $(output)

