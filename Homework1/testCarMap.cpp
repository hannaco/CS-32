#include "CarMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	CarMap m;  // maps strings to doubles
	assert(m.fleetSize() == 0);
	m.addCar("hello");
	assert(m.miles("hello") == 0);
	m.drive("hello", 100);
	ValueType v = 0;
	assert(m.miles("hello") == 100);
	assert(m.fleetSize() == 1);
	m.addCar("world");
	m.print();
	cout << "Passed all tests" << endl;
}