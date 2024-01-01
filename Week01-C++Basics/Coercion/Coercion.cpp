#include <iostream>
#include <iomanip>
#include <limits>

using namespace std; 

int main()
{ 
      int i =-2;
      unsigned int ui=1; 

      cout << "Sum of  "  <<  10  <<  " and " << 'b' 
	      << " is " << 10+'b' << endl;

      cout << "Sum of  int "  <<  i  <<  " and unsigned int " << ui 
	      << " is " << ui +i << endl;

      i = numeric_limits<int>::max();
      cout << i  << " + 2 = " << i +2 << endl; 

      i = numeric_limits<int>::min();
      cout << i  << " - 2 = " << i - 2 << endl; 

      return 0;  
}
