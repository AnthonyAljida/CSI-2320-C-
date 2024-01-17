#include <iostream>
using namespace std;

int main()
{
    // Get input for n
    int n;
    cout << "Enter a value for n: ";
    cin >> n;

    // Display the diamond pattern
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
