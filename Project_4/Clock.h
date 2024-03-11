#ifndef CLOCK_H
#define CLOCK_H

#include <iostream>
#include <iomanip>
#include <string>

class Clock
{
private:
    int hours;
    int minutes;
    int seconds;
    bool twelveHourFormat;

public:
    // Normal constructor
    Clock();
    // Overloaded constructor
    Clock(int hour, int minute, int second);
    // Change to military time
    void toggleFormat();
    // Show the current time
    void show();
    // Move time ahead by one second
    void tick();
};

#endif
