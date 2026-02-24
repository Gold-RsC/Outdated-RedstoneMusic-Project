#include"line.h"
#include<iostream>
using namespace std;
int main(void){
	Ray a,b;
	a(Point(0,0),Point(1,2));
	b(Point(0,0),Point(-1,2));
	cout<<a.angular_bisector(b);
	return 0;
}
