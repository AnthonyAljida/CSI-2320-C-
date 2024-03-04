#include "Clock.h"

int main()
{
    Clock clock1;             // Default constructor
    Clock clock2(15, 59, 50); // Constructor with parameters

    // Test all public member functions
    clock1.show();
    clock2.show();

    clock1.toggleFormat();
    clock2.toggleFormat();

    clock1.tick();
    clock2.tick();

    clock1.show();
    clock2.show();

    return 0;
}
