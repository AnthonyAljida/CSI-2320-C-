#ifndef CITYCLOCK_H
#define CITYCLOCK_H

#include <iostream>
#include <iomanip>
#include <string>

class CityClock
{
private:
    int hours;
    int minutes;
    int seconds;
    bool twelveHourFormat;
    char *cityName;

public:
    // Normal constructor
    CityClock();
    // Destructor
    ~CityClock();
    // Overloaded constructor
    CityClock(int hour, int minute, int second, const char *city);

    // Copy constructor
    CityClock(const CityClock &other);

    // Move constructor
    CityClock(CityClock &&other);

    // Copy assignment operator
    CityClock &operator=(const CityClock &other);

    // Move assignment operator
    CityClock &operator=(CityClock &&other);

    // Type conversion function
    operator std::string() const;

    // Overloaded addition operator
    CityClock operator+(int secondsToAdd) const;

    // Overloaded ostream insertion operator
    friend std::ostream &operator<<(std::ostream &os, const CityClock &clock);

    // Change to military time
    void toggleFormat();
    // Show the current time
    void show();
    // Move time ahead by one second
    void tick();
};

#endif
