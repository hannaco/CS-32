#ifndef NEWMAP_H
#define NEWMAP_H

#include <string>

using KeyType = std::string;
using ValueType = double;

const int DEFAULT_MAX_ITEMS = 240;

class Map
{
public:
	Map();

	Map(int hold);

	Map(const Map& old);

	Map& operator= (const Map& src);

	~Map();

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
	struct keyValue
	{
	public:
		KeyType m_key;
		ValueType m_value;
	};
	keyValue* m_map;
	int num;
	int max;
};

#endif