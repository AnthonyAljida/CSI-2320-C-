#include <iostream>
#include <stdlib.h>
using namespace std;

// Function prototypes
int *duplicate(int *arr, int size);
int *generate(unsigned int n);
void display(int *arr, int size);

int main()
{
    unsigned int size;
    cout << "Enter the size of the dynamic array to create" << endl;
    cin >> size;

    int *generatedArray = generate(size);
    display(generatedArray, size);

    int *dup = duplicate(generatedArray, size);
    display(dup, size * 2);
}

int *duplicate(int *arr, int size)
{
    cout << "Array doubled\n";
    int *twoCopies = new int[size * 2];

    for (int i = 0; i < size; i++)
    {
        twoCopies[2 * i] = arr[i];
        twoCopies[2 * i + 1] = arr[i];
    }

    return twoCopies;
}

int *generate(unsigned int n)
{

    int *randArr = new int[n];
    for (int i = 0; i < n; i++)
    {
        randArr[i] = rand() % 100;
    }

    return randArr;
}

void display(int *arr, int size)
{
    cout << "Displaying elements of dynamic array\n";
    for (int i = 0; i < size; i++)
    {
        if (i != 0 && i % 8 == 0)
        {
            cout << endl;
        }
        cout << "Element " << i << ": " << arr[i] << " ";
    }
    cout << endl;
}