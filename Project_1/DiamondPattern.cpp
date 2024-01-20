#include <iostream>
#include <limits>
using namespace std;

int main()
{
    int n;
    cout << "Enter a value for n: ";

    while (!(cin >> n) || n < 0)
    {

        cin.clear();

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid input! Please try again with valid input: ";
    }
    //    The outer loop is run for the lines from the start to the middle
    for (int i = 0; i <= n; i++)
    {
        // Display blanks
        for (int j = 0; j < n - i; j++)
            cout << " ";

        // Display asterisks
        for (int k = 0; k < 2 * i + 1; k++)
            cout << "*";

        cout << endl;
    }
    // The lines are run from one below the middle to the end
    for (int i = n - 1; i >= 0; i--)
    {
        // Display blanks
        for (int j = 0; j < n - i; j++)
            cout << " ";

        // Display asterisks
        for (int k = 0; k < 2 * i + 1; k++)
            cout << "*";

        cout << endl;
    }

    return 0;
}
