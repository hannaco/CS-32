#include<functional>
#include<list>
#include<iostream>
// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);


	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
	struct Bucket
	{
		Bucket(KeyType key, ValueType value) :k(key), v(value) {}
		KeyType k;
		ValueType v;
	};
	unsigned int getBucketNum(const KeyType& k) const
	{
		unsigned int hasher(const KeyType & key); // prototype
		unsigned int h = hasher(k);
		return h;
	}
	double maxLoad;
	int numElements;
	int maxBuckets;
	bool inMap(KeyType key);
	std::list<Bucket>* m_hash;
};

template<typename KeyType, typename ValueType>
bool ExpandableHashMap<KeyType, ValueType>::inMap(KeyType key)
{
	unsigned int h = getBucketNum(key);
	unsigned int bucketNum = h % maxBuckets;
	typename std::list<Bucket>::iterator it;
	it = m_hash[bucketNum].begin();
	while (it != m_hash[bucketNum].end())
	{
		if (it->k == key)
			return true;
		it++;
	}
	return false;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
	maxLoad = maximumLoadFactor;
	maxBuckets = 8;
	numElements = 0;
	m_hash = new std::list<Bucket>[maxBuckets];
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
	delete[] m_hash;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	delete[] m_hash;
	numElements = 0;
	maxBuckets = 8;
	m_hash = new std::list<Bucket>[maxBuckets];
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	return numElements;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	unsigned int h = getBucketNum(key);
	unsigned int bucketNum = h % maxBuckets;
	if (inMap(key))
	{
		typename std::list<Bucket>::iterator it = m_hash[bucketNum].begin();
		while (it != m_hash[bucketNum].end())
		{
			if (it->k == key)
			{
				it->v = value;
				return;
			}
			it++;
		}
	}
	else
	{
		Bucket b(key, value);
		m_hash[bucketNum].push_back(b);
		numElements++;
	}
	if ((double)numElements / maxBuckets > maxLoad)
	{
		std::list<Bucket>* m_temp = new std::list<Bucket>[maxBuckets * 2];
		for (int i = 0; i < maxBuckets; i++)
		{
			if (m_hash[i].empty())
				continue;
			else
			{
				typename std::list<Bucket>::iterator it = m_hash[i].begin();
				while (it != m_hash[i].end())
				{
					KeyType curK = it->k;
					ValueType curV = it->v;
					Bucket buck(curK, curV);
					unsigned int newH = getBucketNum(curK);
					unsigned int newBucket = newH % (maxBuckets * 2);
					m_temp[newBucket].push_back(buck);
					it++;
				}
			}
		}
		delete[] m_hash;
		m_hash = m_temp;;
		maxBuckets *= 2;
	}
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
	unsigned int h = getBucketNum(key);
	unsigned int bucketNum = h % maxBuckets;
	typename std::list<Bucket>::iterator it = m_hash[bucketNum].begin();
	while (it != m_hash[bucketNum].end())
	{
		if (it->k == key)
			return &it->v;
		it++;
	}
	return nullptr;
}
