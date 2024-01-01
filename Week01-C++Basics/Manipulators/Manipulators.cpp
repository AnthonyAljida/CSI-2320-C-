#include <iostream>
#include <iomanip>

using namespace std; 

int main()
{ 
        const double PI = 3.14; 
	double radius, circumference, area; 

	cout <<  "Enter radius of the circle "; 
	cin >> radius; 

	circumference = 2*PI*radius;
	area = PI*radius*radius; 

        cout << fixed << showpoint << setprecision(3); 
        cout << endl; 

	cout << setw(16) << left  <<  "Radius" << "=" << setw(10) << right << radius << endl; 
	cout << setw(16) << left  <<  "Circumference" << "="  << setw(10) << right << circumference << endl; 
	cout << setw(16) << left  <<  "Area" << "="  << setw(10) << right << area << endl; 

      return 0;  
}
