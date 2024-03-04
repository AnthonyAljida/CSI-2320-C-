CXX = g++
CXXFLAGS = -Wall -std=c++11

main: main.o Clock.o
	$(CXX) $(CXXFLAGS) -o main main.o Clock.o

main.o: main.cpp Clock.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Clock.o: Clock.cpp Clock.h
	$(CXX) $(CXXFLAGS) -c Clock.cpp

clean:
	rm -f *.o main
