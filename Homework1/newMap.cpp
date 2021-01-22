#include "newMap.h"
#include<iostream>

Map::Map()
{
	m_map = new keyValue[DEFAULT_MAX_ITEMS];
	max = DEFAULT_MAX_ITEMS;
	num = 0;
}

Map::Map(int hold)
{
	if (hold < 0)
	{
		std::cout << "Array capacity cannot be negative!" << std::endl;
		exit(0);
	}
	m_map = new keyValue[hold];
	max = hold;
	num = 0;
}

Map::Map(const Map& old)
{
	max = old.max;
	num = old.num;
	m_map = new keyValue[max];
	for (int i = 0; i < num; i++)
	{
		m_map[i].m_key = old.m_map[i].m_key;
		m_map[i].m_value = old.m_map[i].m_value;
	}
}

Map& Map::operator= (const Map& src)
{
	delete [] m_map;
	max = src.max;
	num = src.num;
	m_map = new keyValue[max];
	for (int i = 0; i < num; i++)
	{
		m_map[i].m_key = src.m_map[i].m_key;
		m_map[i].m_value = src.m_map[i].m_value;
	}
	return *this;
}

Map::~Map()
{
	delete [] m_map;
}

bool Map::empty() const
{
	if (num == 0)
		return true;
	return false;
}

int Map::size() const
{
	return num;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).
{
	if (num == max)
		return false;
	for (int i = 0; i < num; i++)
	{
		if (m_map[i].m_key == key)
			return false;
	}
	m_map[num].m_key = key;
	m_map[num].m_value = value;
	num++;
	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
{
	for (int i = 0; i < num; i++)
	{
		if (m_map[i].m_key == key)
		{
			m_map[i].m_value = value;
			return true;
		}
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value that it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).
{
	bool canUpdate = update(key, value);
	if (canUpdate)
		return true;
	else
	{
		bool canInsert = insert(key, value);
		if (canInsert)
			return true;
	}
	return false;
}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
{
	for (int i = 0; i < num; i++)
	{
		if (m_map[i].m_key == key)
		{
			for (int j = i; i < num - 1; i++)
			{
				m_map[j].m_key = m_map[j + 1].m_key;
				m_map[j].m_value = m_map[j + 1].m_value;
			}
			num--;
			return true;
		}
	}
	return false;
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the map, otherwise
// false.
{
	for (int i = 0; i < num; i++)
	{
		if (m_map[i].m_key == key)
			return true;
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
// If key is equal to a key currently in the map, set value to the
// value in the map which that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
	for (int i = 0; i < num; i++)
	{
		if (m_map[i].m_key == key)
		{
			value = m_map[i].m_value;
			return true;
		}
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
{
	if (i < 0 || i >= num)
		return false;
	key = m_map[i].m_key;
	value = m_map[i].m_value;
	return true;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
	keyValue* temp;
	temp = m_map;
	*m_map = *other.m_map;
	*other.m_map = *temp;
	int tempSize = num;
	num = other.num;
	other.num = tempSize;
	int tempMax = max;
	max = other.max;
	other.max = tempMax;
}