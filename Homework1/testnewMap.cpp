#include "newMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Map m;  // maps strings to doubles
	assert(m.empty());
	ValueType v = -1234.5;
	assert(!m.get("abc", v) && v == -1234.5); // v unchanged by get failure
	m.insert("xyz", 9876.5);
	assert(m.size() == 1);
	KeyType k = "hello";
	assert(m.get(0, k, v) && k == "xyz" && v == 9876.5);
	Map n;
	n.insert("hello", 1);
	n.insert("world", 2);
	ValueType x = 0;
	assert(n.size() == 2);
	n.update("hello", 3);
	n.get("hello", x);
	assert(x == 3);
	n.swap(m);
	assert(n.size() == 1 && m.size() == 2);
	Map a = m;
	assert(a.size() == m.size());
	a = n;
	assert(a.size() == n.size());
	cout << "Passed all tests" << endl;
}
