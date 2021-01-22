//#include "Map.h"
//#include <iostream>
//#include <cassert>
//using namespace std;
//
//bool combine(const Map& m1, const Map& m2, Map& result);
//void reassign(const Map& m, Map& result);
//
//int main()
//{
//	Map m;  // maps strings to doubles
//	assert(m.empty());
//	ValueType v = -1234.5;
//	assert(!m.get("abc", v) && v == -1234.5); // v unchanged by get failure
//	m.insert("xyz", 9876.5);
//	assert(m.size() == 1);
//	KeyType k = "hello";
//	assert(m.get(0, k, v) && k == "xyz" && v == 9876.5);
//	Map n;
//	n.insert("hello", 1);
//	n.insert("world", 2);
//	ValueType x = 0;
//	assert(n.size() == 2);
//	n.update("hello", 3);
//	n.get("hello", x);
//	assert(x == 3);
//	n.swap(m);
//	assert(n.size() == 1 && m.size() == 2);
//	Map a = m;
//	assert(a.size() == m.size());
//	a = n;
//	assert(a.size() == n.size());
//	Map one;
//	one.insert("michelle", 8);
//	one.insert("adelpha", 7);
//	one.insert("hanna", 14);
//	Map two;
//	two.insert("david", 2);
//	two.insert("steven", 35);
//	two.insert("roger", 11);
//	Map oneMod;
//	reassign(one, oneMod);
//	ValueType check1 = 8;
//	ValueType check2 = 7;
//	ValueType check3 = 14;
//	assert(oneMod.get("michelle", check1) && check1 != 8);
//	assert(oneMod.get("adelpha", check2) && check2 != 7);
//	assert(oneMod.get("hanna", check3) && check3 != 14);
//	combine(one, two, oneMod);
//	assert(oneMod.size() == one.size() + two.size());
//	assert(oneMod.contains("hanna") && oneMod.contains("michelle") && oneMod.contains("adelpha") && oneMod.contains("david") && oneMod.contains("steven") && oneMod.contains("roger"));
//	Map result;
//	Map h;
//	h.insert("sally", 6);
//	h.insert("bob", 12);
//	Map j;
//	j.insert("sally", 6);
//	j.insert("mary", 7);
//	combine(h, j, result);
//	assert(result.size() == 3);
//	Map test;
//	Map three;
//	Map four;
//	three.insert("this", 1);
//	three.insert("is", 2);
//	three.insert("for", 3);
//	four.insert("rachel", 4);
//	four.insert("is", 5);
//	assert(!combine(three, four, test) && test.size() == 1);
//	cout << "Passed all tests" << endl;
//}
//
