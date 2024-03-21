#include "CityClock.h"
using namespace std;
CityClock::CityClock()
{
    hours = 0;
    minutes = 0;
    seconds = 0;
    twelveHourFormat = true;
}

CityClock::CityClock(int h, int m, int s)
{
    if (h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60)
    {
        hours = h;
        minutes = m;
        seconds = s;
        twelveHourFormat = true;
    }
    else
    {
        cout << "Invalid time input. Please double check your time arguments\n";
    }
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
    CityClock CityClock1;
    CityClock CityClock2(19, 34, 2);

    // Test all public member functions
    CityClock1.show();
    CityClock2.show();

    CityClock1.tick();
    CityClock2.tick();

    CityClock1.show();
    CityClock2.show();

    CityClock1.toggleFormat();
    CityClock2.toggleFormat();

    CityClock1.tick();
    CityClock2.tick();

    CityClock1.show();
    CityClock2.show();

    return 0;
}
