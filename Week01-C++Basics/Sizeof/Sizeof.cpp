#include <iostream>
#include <iomanip>

using namespace std; 

int main()
{

      cout << setw(24) << left <<  "Type Name" << setw(8) << right << "Bytes" << endl;  
      cout << setw(24) << left <<  "short int" << setw(8) << right << sizeof (short int) << endl;  
      cout << setw(24) << left <<  "unsigned short int" << setw(8) << right << sizeof (unsigned short int) << endl;  
      cout << setw(24) << left <<  "int" << setw(8) << right << sizeof (int) << endl;  
      cout << setw(24) << left <<  "unsigned int" << setw(8) << right << sizeof (unsigned  int) << endl;  
      cout << setw(24) << left <<  "long int" << setw(8) << right << sizeof (long int) << endl;  
      cout << setw(24) << left <<  "unsigned long int" << setw(8) << right << sizeof (unsigned long  int) << endl;  
      cout << setw(24) << left <<  "long long int" << setw(8) << right << sizeof (long long int) << endl;  
      cout << setw(24) << left <<  "unsigned long long int" << setw(8) << right << sizeof (unsigned long long  int) << endl;  
      cout << setw(24) << left <<  "char" << setw(8) << right << sizeof (char) << endl;  
      cout << setw(24) << left <<  "bool" << setw(8) << right << sizeof (bool) << endl;  
      cout << setw(24) << left <<  "float" << setw(8) << right << sizeof (float) << endl;  
      cout << setw(24) << left <<  "double" << setw(8) << right << sizeof (double) << endl;  
      cout << setw(24) << left <<  "long double" << setw(8) << right << sizeof (long  double) << endl;  

      return 0;  
}
