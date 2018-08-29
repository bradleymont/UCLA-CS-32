//
//  CarMap.cpp
//  CarMap
//
//  Created by Bradley Mont on 1/20/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#include "CarMap.h"
#include "Map.h"
#include <iostream>
#include <string>
CarMap::CarMap()       // Create an empty car map.
{
    //Map constructor is automatically called
}

bool CarMap::addCar(std::string license)
// If a car with the given license plate is not currently in the map,
// and there is room in the map, add an entry for that car recording
// that it has 0 gallons of gas in it, and return true.  Otherwise,
// make no change to the map and return false.
{
    return m_map.insert(license, 0);
}

double CarMap::gas(std::string license) const
// If a car with the given license plate is in the map, return the
// number of gallons of gas in its tank; otherwise, return -1.
{
    double numGallons = -1;
    m_map.get(license, numGallons);
    return numGallons;
}

bool CarMap::addGas(std::string license, double gallons)
// If no car with the given license plate is in the map or if
// gallons is negative, make no change to the map and return
// false.  Otherwise, increase the number of gallons of gas in the
// indicated car by the gallons parameter and return true.
{
    if (gallons < 0)
    {
        return false;
    }
    
    double currrentGallons = 0;
    if (m_map.get(license, currrentGallons)) //will return true if that license plate is in the map
    {
        m_map.update(license, currrentGallons + gallons);
        return true;
    }
    
    return false;   //gallons is positive, but no car with that license plate is in the map
}

bool CarMap::useGas(std::string license, double gallons)
// If no car with the given license plate is in the map or if
// gallons is negative or if gallons > gas(), make no change to the
// map and return false.  Otherwise, decrease the number of gallons
// of gas in the indicated car by the gallons parameter and return
// true.
{
    if (gallons < 0    ||    gallons > gas(license))
    {
        return false;
    }
    
    double currrentGallons = 0;
    if (m_map.get(license, currrentGallons)) //will return true if that license plate is in the map
    {
        m_map.update(license, currrentGallons - gallons);
        return true;
    }
    
    return false;   //gallons is positive, but no car with that license plate is in the map
}

int CarMap::fleetSize() const  // Return the number of cars in the CarMap.
{
    return m_map.size();
}

void CarMap::print() const
// Write to cout one line for every car in the map.  Each line
// consists of the car's license plate, followed by one space,
// followed by the number of gallons in that car's tank.  Write
// no other text.
{
    std::string key = "";
    double value = 0;
    for (int i = 0; i < fleetSize(); i++)
    {
        m_map.get(i, key, value);
        std::cout << key << " " << value << std::endl;
    }
}
