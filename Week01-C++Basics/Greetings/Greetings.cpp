#include <iostream>
#include <string>

using namespace std; 

int main()
{
      string first, last, city;

      cout << "What is your first name?" << endl;
      cin >> first; 

      cout << "What is your last name?\n";
      cin >> last; 
      cin.ignore(9223372036854775807, '\n');

      cout << "Which city do you live in?" << endl;
      getline(cin, city); 

      cout << "Welcome " << first << " " << last << " from " << city << ".\n";

      return 0;  
}
