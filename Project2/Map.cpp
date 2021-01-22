#include "Map.h"

Map::Map()               // Create an empty map (i.e., one with no key/value pairs)
{
	num = 0;
	head = nullptr;
	tail = nullptr;
}

Map::~Map()				//deallocate all nodes in the linked list
{
	Pair* p;
	p = head;
	while (p != nullptr)		//traverse linked list
	{
		Pair* d = p->next;		//new pointer to Pair after p
		delete p;				//delete the current pair
		p = d;					//assign p to be the one after the deleted Pair
	}
}

Map::Map(const Map& old)	//enough new nodes must be allocated to hold a duplicate of the original list
{
	num = 0;				//initializing
	head = nullptr;
	tail = nullptr;
	Pair* src; 
	src = old.head;
	while (src != nullptr)		//traversing the old linked list
	{
		KeyType curK = src->m_key;
		ValueType curV = src->m_value;
		insert(curK, curV);		//inserting the values from the old pair into the new Map
		src = src->next;		//moving onto the next Pair
	}
}

Map& Map::operator= (const Map& old)
//When an existing Map (the left-hand side) is assigned the value of 
//another Map (the right-hand side), the result must be that the left-hand
//side object is a duplicate of the right-hand side object, with no memory
//leak of list nodes (i.e. no list node from the old value of the left-hand 
//side should be still allocated yet inaccessible).
{
	if (this != &old)
	{
		Map temp(old);
		swap(temp);
	}
	return *this;
}

bool Map::empty() const  // Return true if the map is empty, otherwise false.
{
	if (num == 0)
		return true;
	return false;
}

int Map::size() const    // Return the number of key/value pairs in the map.
{
	return num;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full.
{
	Pair* cur;
	cur = head;
	if (contains(key) == false)			//if no Pair that matches key, return false
	{
		Pair* p;
		p = new Pair;
		p->m_key = key;				//make a new Pair that has values equal to key and value
		p->m_value = value;
		if (head == nullptr)		//if it is the first element
		{							//then there is no element before or after it, so next and prev are nullptrs
			p->next = nullptr;		//head and tail will then just point to this one element
			p->prev = nullptr;
			head = p;
			tail = p;
		}
		else
		{
			p->next = head;			//otherwise, insert p as the first element, setting its next to be the previous first element
			p->prev = nullptr;		//no prev because it is the first element
			head->prev = p;			//the former first element's prev now points to p		
			head = p;				//head points to p
		}
		num++;						//increment number of elements in linked list
		return true;
	}
	else
		return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
{
	Pair* cur;
	cur = head;	
	while (cur != nullptr)			//traverse linked list
	{
		if (cur->m_key == key)		//if the current Pair's key matches parameter
		{
			cur->m_value = value;	//change m_value and exit the loop
			return true;
		}
		cur = cur->next;
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value that it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).
{
	if (update(key, value))			//change m_value if key is in Map
		return true;
	else if (insert(key, value))	//else, try to insert key and value into Map
		return true;
	return false;
}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
{
	if (head->next == nullptr && head != nullptr && head->m_key == key)	//if there is only one element and it is the correct one
	{
		Pair* gone = head;					//temp Pair pointing to the one
		head = nullptr;						//set head and tail to nullptr
		tail = nullptr;
		delete gone;						//delete the target element					
		num--;								//decrement number of elements to zero
		return true;						//exit function
	}
	else if (head->m_key == key)					//if 2+ elements, and target is the first
	{
		Pair* gone = head;					//temp Pair pointing to it
		head = gone->next;					//set head to the element after the target
		head->prev = nullptr;				//new head's prev now points to nullptr
		delete gone;						//delete and decrement
		num--;
		return true;						//exit function
	}
	else if (tail->m_key == key)
	{
		Pair* gone = tail;
		tail = gone->prev;
		tail->next = nullptr;
		delete gone;
		num--;
		return true;
	}
	Pair* cur;
	cur = head;
	while (cur != nullptr)					//traverse linked list
	{
		if (cur->next != nullptr && cur->next->m_key == key)		//if element after current is the target, break out of loop
			break;
		cur = cur->next;
	}
	if (cur == nullptr)						//if target isn't in list, return false
		return false;
	Pair* willDelete = cur->next;			//temp Pair to target
	cur->next = willDelete->next;			//setting Pair before target's next to be the one after target
	willDelete->next->prev = cur;			//setting the element after target's prev to be the one before target
	delete willDelete;						//delete and decrement
	num--;
	return true;
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the map, otherwise
// false.
{
	Pair* cur = head;
	while (cur != nullptr)					//traverse linked list
	{	
		if (cur->m_key == key)				//if it is in linked list, return true
			return true;
		cur = cur->next;
	}
		return false;						//otherwise return false
}

bool Map::get(const KeyType& key, ValueType& value) const
// If key is equal to a key currently in the map, set value to the
// value in the map which that key maps to and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
	Pair* cur;
	cur = head;
	while (cur != nullptr)
	{
		if (cur->m_key == key)				//if the current element's m_key matches key, reassign its m_value
		{
			value = cur->m_value;
			return true;					//exit function
		}
		cur = cur->next;
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.
{
	if (i < 0 || i >= num)					//making sure i is not negative, and not greater than number of elements
		return false;
	Pair* cur;
	cur = head;
	if (i != 0)
	{
		for (int j = 0; j < i; j++)				//getting the i+1 th Pair in linked list
		{
			cur = cur->next;
		}
	}
	key = cur->m_key;						//set key to m_key and value to m_value
	value = cur->m_value;
	return true;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
	Pair* curH = other.head;
	other.head = head;
	head = curH;
	int tempNum = other.num;
	other.num = num;
	num = tempNum;
	Pair* curT = other.tail;
	other.tail = tail;
	tail = curT;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
	KeyType m1K;
	ValueType m1V;
	KeyType m2K;
	ValueType m2V;
	for (int i = 0; i < m1.size(); i++)
	{
		m1.get(i, m1K, m1V);
		if (!m2.contains(m1K))
		{
			if (result.contains(m1K))
				break;
			result.insert(m1K, m1V);
		}
		else if (m2.contains(m1K))
		{
			m2.get(m1K, m2V);
			if (m2V == m1V)
				result.insert(m1K, m1V);
			else
				return false;
		}
	}
	for (int j = 0; j < m2.size(); j++)
	{
		m2.get(j, m2K, m2V);
		if (result.contains(m2K))
			break;
		else
			result.insert(m2K, m2V);
	}
	return true;
}

void reassign(const Map& m, Map& result)
{
	result = m;
	ValueType VLast;
	KeyType KLast;
	ValueType VFirst;
	KeyType KFirst;
	ValueType mV1;
	KeyType mK1;
	ValueType mV2;
	KeyType mK2;
	for (int i = 0; i < m.size() - 1; i++)
	{
		m.get(i, mK1, mV1);
		m.get(i + 1, mK2, mV2);
		result.update(mK1, mV2);
	}
	m.get(m.size() - 1, KLast, VLast);
	m.get(0, KFirst, VFirst);
	result.update(KLast, VFirst);
}