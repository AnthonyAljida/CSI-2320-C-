#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>
#include <bits/stdc++.h>
#include <map>
using namespace std;
ifstream processFile();
void readCharactersToArray(ifstream &file, int arr[]);
void sortInAscending(int arr[], int length);
int main()
{
    // Sets default values to 0
    int frequencies[26] = {};
    std::multimap<int, char> ex;
    // Returns the opened file
    ifstream openedFile = processFile();

    // Changes values of array to frequency of the letters, includes lower and uppercase version and ignores non-alphabet characters
    readCharactersToArray(openedFile, frequencies);

    // Length of array
    int length = sizeof(frequencies) / sizeof(frequencies[0]);
    // Display letters in order from A-Z along with frequencies
    for (int i = 0; i < length; i++)
    {
        char letter = static_cast<char>(i + 'A');
        cout << "Frequency of " << letter << "'s: " << frequencies[i] << endl;
        ex.insert(pair<int, char>(frequencies[i], letter));
    }
    cout << endl;

    // Sort array ascending
    sortInAscending(frequencies, length);

    for (auto itr = ex.begin(); itr != ex.end(); ++itr)
    {
        cout << "Frequency of " << itr->second << "'s after sorting in ascending order of frequencies: " << itr->first << endl;
    }
    return 0;
}
ifstream processFile()
{
    string fileName;
    ifstream file;
    char c;

    cout << "Enter the name of your file: " << endl;
    cin >> fileName;

    file.open(fileName, ios::in);

    while (!file.is_open())
    {
        cout << "You entered the wrong name of your file, please double-check the name" << endl;
        cin >> fileName;
        file.open(fileName);
    }
    return file;
}
void readCharactersToArray(ifstream &fileOpened, int arr[])
{
    while (fileOpened.good())
    {
        char currentLetter = fileOpened.get();
        char Upper = toupper(currentLetter);
        int ASCIIOfLetter = static_cast<int>(Upper);
        if (ASCIIOfLetter >= 65 && ASCIIOfLetter <= 90)
        {
            arr[ASCIIOfLetter - 'A']++;
        }
    }
}

void sortInAscending(int arr[], int length)
{
    sort(arr, arr + length);
}