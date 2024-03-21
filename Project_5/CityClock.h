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
    CityClock(int hour, int minute, int second);
    // Change to military time
    void toggleFormat();
    // Show the current time
    void show();
    // Move time ahead by one second
    void tick();
};

#endif
