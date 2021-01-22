#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include<string>
#include "OurType.h"
using KeyType = OurType;
using ValueType = std::string;

class Map
{
public:
	Map();
	~Map();
	Map(const Map& old);
	Map& operator= (const Map& old);
	bool empty() const;
	int size() const;
	bool insert(const KeyType& key, const ValueType& value);
	bool update(const KeyType& key, const ValueType& value);
	bool insertOrUpdate(const KeyType& key, const ValueType& value);
	bool erase(const KeyType& key);
	bool contains(const KeyType& key) const;
	bool get(const KeyType& key, ValueType& value) const;
	bool get(int i, KeyType& key, ValueType& value) const;
	void swap(Map& other);
private:
	struct Pair
	{
	public:
		KeyType m_key;
		ValueType m_value;
		Pair* next;
		Pair* prev;
	};

	int num;
	Pair* head;
	Pair* tail;
};

#endif