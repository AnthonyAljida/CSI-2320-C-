#include "Clock.h"

Clock::Clock()
{
    hours = 0;
    minutes = 0;
    seconds = 0;
    twelveHours = true; // Default format is 12-hour
}

Clock::Clock(int h, int m, int s)
{
    if (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60)
    {
        hours = h;
        minutes = m;
        seconds = s;
        twelveHours = true; // Default format is 12-hour
    }
    else
    {
        std::cerr << "Invalid time input.\n";
        // Default time if input is invalid
        hours = 0;
        minutes = 0;
        seconds = 0;
        twelveHours = true;
    }
}

void Clock::toggleFormat()
{
    twelveHours = !twelveHours;
}

void Clock::show()
{
    std::string format = (twelveHours) ? "12-hour" : "24-hour";
    std::cout << "Time: ";
    if (twelveHours)
    {
        if (hours >= 12)
            std::cout << std::setfill('0') << std::setw(2) << hours - 12;
        else
            std::cout << std::setfill('0') << std::setw(2) << hours;
    }
    else
    {
        std::cout << std::setfill('0') << std::setw(2) << hours;
    }
    std::cout << ":" << std::setfill('0') << std::setw(2) << minutes
              << ":" << std::setfill('0') << std::setw(2) << seconds;
    if (twelveHours)
    {
        if (hours >= 12)
            std::cout << " PM";
        else
            std::cout << " AM";
    }
    std::cout << " (" << format << " format)\n";
}

void Clock::tick()
{
    seconds++;
    if (seconds >= 60)
    {
        seconds = 0;
        minutes++;
        if (minutes >= 60)
        {
            minutes = 0;
            hours++;
            if (hours >= 24)
            {
                hours = 0;
            }
        }
    }
}
