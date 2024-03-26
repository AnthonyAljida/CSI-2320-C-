CXX = g++
CXXFLAGS = -Wall -std=c++11

all: main clean

main: CityClock.o
	$(CXX) $(CXXFLAGS) -o main CityClock.o

CityClock.o: CityClock.cpp CityClock.h
	$(CXX) $(CXXFLAGS) -c CityClock.cpp

clean:
	rm -f *.o
