#include "CityClock.h"
#include <cstring>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

CityClock::CityClock()
{
    hours = 0;
    minutes = 0;
    seconds = 0;
    cityName = new char[strlen("Rochester, Michigan") + 1];
    strcpy(cityName, "Rochester, Michigan");
    twelveHourFormat = true;
}
CityClock::CityClock(int h, int m, int s, const char *city)
{
    if (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60)
    {
        hours = h;
        minutes = m;
        seconds = s;
        twelveHourFormat = true;
        cityName = new char[strlen(city) + 1];
        strcpy(cityName, city);
    }
    else
    {
        // Print an error message and set cityName to nullptr
        std::cerr << "Invalid time input or city name is nullptr. Please double check your arguments.\n";
        cityName = nullptr;
    }
}
CityClock::CityClock(const CityClock &origin)
{
    hours = origin.hours;
    minutes = origin.minutes;
    seconds = origin.seconds;
    twelveHourFormat = origin.twelveHourFormat;
    cityName = new char[strlen(origin.cityName) + 1];
    strcpy(cityName, origin.cityName);
}

CityClock::CityClock(CityClock &&origin)
{
    hours = origin.hours;
    minutes = origin.minutes;
    seconds = origin.seconds;
    twelveHourFormat = origin.twelveHourFormat;
    cityName = origin.cityName;
    origin.cityName = nullptr; // Invalidate the pointer in the source object
}
CityClock &CityClock::operator=(const CityClock &origin)
{
    if (this == &origin) // Self-assignment check
        return *this;

    // Copy values
    hours = origin.hours;
    minutes = origin.minutes;
    seconds = origin.seconds;
    twelveHourFormat = origin.twelveHourFormat;
    delete[] cityName; // Free existing memory
    cityName = new char[strlen(origin.cityName) + 1];
    strcpy(cityName, origin.cityName);

    return *this;
}
CityClock::~CityClock()
{
    delete[] cityName;
}

CityClock &CityClock::operator=(CityClock &&origin)
{
    if (this == &origin) // Self-assignment check
        return *this;

    // Move values
    hours = origin.hours;
    minutes = origin.minutes;
    seconds = origin.seconds;
    twelveHourFormat = origin.twelveHourFormat;
    delete[] cityName; // Free existing memory
    cityName = origin.cityName;

    // Invalidate the pointer in the source object
    origin.cityName = nullptr;

    return *this;
}

CityClock::operator std::string() const
{
    // std::ostringstream oss;
    // oss << std::setfill('0') << std::setw(2) << hours << ":"
    //     << std::setw(2) << minutes << ":"
    //     << std::setw(2) << seconds;
    // std::string timeStr = std::string(cityName) + " - " + oss.str();
    // return twelveHourFormat ? timeStr : timeStr + (hours < 12 ? " AM" : " PM");
    if (cityName != nullptr)
    {
        string time = cityName;
        time += " - ";
        if (twelveHourFormat)
        {
            if (hours == 0)
                time += "12";
            else if (hours > 12)
                time += to_string(hours - 12);
            else
                time += to_string(hours);
        }
        else
        {
            time += to_string(hours);
        }
        time += ":";
        if (minutes < 10)
            time += "0";
        time += to_string(minutes);
        time += ":";
        if (seconds < 10)
            time += "0";
        time += to_string(seconds);
        if (twelveHourFormat)
        {
            if (hours >= 12)
            {
                time += " PM";
            }
            else
            {

                time += " AM";
            }
        }
        return time;
    }
    else
    {
        // Return an empty string or throw an exception
        return "";
    }
}

CityClock CityClock::operator+(int secondsToAdd) const
{
    CityClock result = *this;
    int totalSeconds = hours * 3600 + minutes * 60 + seconds + secondsToAdd;

    // Adjust hours, minutes, and seconds if necessary
    result.hours = (totalSeconds / 3600) % 24;
    result.minutes = (totalSeconds / 60) % 60;
    result.seconds = totalSeconds % 60;

    return result;
}

ostream &operator<<(ostream &os, const CityClock &clock)
{
    os << static_cast<std::string>(clock); // Convert CityClock object to std::string
    return os;
}

void CityClock::toggleFormat()
{
    twelveHourFormat = !twelveHourFormat;
}

void CityClock::show()
{
    cout << "Current time: ";
    if (twelveHourFormat)
    {
        if (hours == 0)
        {
            cout << "12";
        }
        else if (hours >= 12)
            cout << setfill('0') << setw(2) << hours - 12;
        else
            cout << setfill('0') << setw(2) << hours;
    }
    else
    {
        cout << setfill('0') << setw(2) << hours;
    }
    cout << ":" << setfill('0') << setw(2) << minutes
         << ":" << setfill('0') << setw(2) << seconds;
    if (twelveHourFormat)
    {
        if (hours >= 12)
            cout << " PM";
        else
            cout << " AM";
    }
    string format = (twelveHourFormat) ? "12-hour" : "24-hour";
    cout << " (" << format << " format)\n";
}

void CityClock::tick()
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

int main()
{
    // Create a CityClock object using the default constructor
    CityClock clock1;
    std::cout << clock1 << std::endl;

    // Create a CityClock object using the parameterized constructor
    CityClock clock2(13, 35, 2, "New York");
    std::cout << clock2 << std::endl;

    // Use the copy constructor
    CityClock clock3 = clock2;
    std::cout << clock3 << std::endl;

    // Use the move constructor
    CityClock clock4 = std::move(clock1);
    std::cout << clock4 << std::endl;

    // Use the copy assignment operator
    clock1 = clock2;
    std::cout << clock1 << std::endl;

    // Use the move assignment operator
    clock2 = std::move(clock3);
    std::cout << clock2 << std::endl;

    // Use the addition operator
    clock3 = clock4 + 3600; // Add one hour
    std::cout << clock3 << std::endl;

    // Use the tick function
    clock4.tick();
    std::cout << clock4 << std::endl;

    // Use the toggleFormat function
    clock1.toggleFormat();
    std::cout << clock1 << std::endl;

    return 0;
}