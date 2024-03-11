CXX = g++
CXXFLAGS = -Wall -std=c++11

all: main clean

main: Clock.o
	$(CXX) $(CXXFLAGS) -o main Clock.o

Clock.o: Clock.cpp Clock.h
	$(CXX) $(CXXFLAGS) -c Clock.cpp

clean:
	rm -f *.o
