#include <iostream>
#include "CarMap.h"

CarMap::CarMap() // Create an empty car map.
{

}

bool CarMap::addCar(KeyType license)
// If a car with the given license plate is not currently in the map, 
// and there is room in the map, add an entry for that car recording
// that it has been driven 0 miles, and return true.  Otherwise,
// make no change to the map and return false.
{
	return m_carmap.insert(license, 0);
}

ValueType CarMap::miles(KeyType license) const
// If a car with the given license plate is in the map, return how
// many miles it has been driven; otherwise, return -1.
{
	ValueType miles;
	if (!m_carmap.contains(license))
		return -1;
	m_carmap.get(license, miles);
	return miles;
}

bool CarMap::drive(KeyType license, ValueType distance)
// If no car with the given license plate is in the map or if
// distance is negative, make no change to the map and return
// false.  Otherwise, increase by the distance parameter the number
// of miles the indicated car has been driven and return true.
{
	ValueType miles;
	if (m_carmap.contains(license))
	{
		m_carmap.get(license, miles);
		if (miles < 0)
			return false;
		miles += distance;
		m_carmap.update(license, miles);
		return true;
	}
	return false;
}

int CarMap::fleetSize() const // Return the number of cars in the CarMap.
{
	return m_carmap.size();
}

void CarMap::print() const
// Write to cout one line for every car in the map.  Each line
// consists of the car's license plate, followed by one space,
// followed by the number of miles that car has been driven.  Write
// no other text.  The lines need not be in any particular order.
{
	KeyType plate;
	ValueType miles;
	for (int i = 0; i < m_carmap.size(); i++)
	{
		m_carmap.get(i, plate, miles);
		std::cout << plate << " " << miles << std::endl;
	}
}
