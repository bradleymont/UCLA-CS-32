//
//  newMap.cpp
//  NewMap
//
//  Created by Bradley Mont on 1/21/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#include "newMap.h"
#include <string>

Map::Map()         // Create an empty map (i.e., one with no key/value pairs)
{
    m_size = 0;
    m_maxSize = DEFAULT_MAX_ITEMS;
    m_pairs = new KeyValuePair[m_maxSize];
}

Map::Map(int maxSize)   //constructor with parameters
{
    m_size = 0;
    m_maxSize = maxSize;
    m_pairs = new KeyValuePair[m_maxSize];
}

Map::Map(const Map &src)    //copy constructor
{
    m_size = src.m_size;
    m_maxSize = src.m_maxSize;
    
    m_pairs = new KeyValuePair[m_maxSize];
    
    for (int j = 0; j < m_size; j++)
    {
        m_pairs[j] = src.m_pairs[j];
    }
}

Map::~Map()     //destructor
{
    delete [] m_pairs;
}

Map& Map::operator= (const Map &src)    //overloaded assignment operator
{
    if (&src == this)
    {
        return (*this);
    }
    
    delete [] m_pairs;
    
    m_size = src.m_size;
    m_maxSize = src.m_maxSize;
    m_pairs = new KeyValuePair[m_maxSize];
    
    for (int j = 0; j < m_size; j++)
    {
        m_pairs[j] = src.m_pairs[j];
    }
    
    return (*this);
}

bool Map::empty() const  // Return true if the map is empty, otherwise false.
{
    return (m_size == 0);
}

int Map::size() const    // Return the number of key/value pairs in the map.
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).
{
    if ( ( ! contains(key))  &&  m_size <  m_maxSize )
    {
        KeyValuePair newPair;
        newPair.key = key;
        newPair.value = value;
        m_pairs[m_size] = newPair;
        m_size++;
        return true;
    }
    
    return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
{
    if (!contains(key))
    {
        return false;
    }
    
    int position = positionOfKey(key);
    m_pairs[position].value = value;
    
    return true;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).
{
    if (insert(key, value))
    {
        return true;
    }
    else if (update(key, value))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
{
    if (!contains(key))
    {
        return false;
    }
    
    //swap the key with the last key. remove the last key
    int position = positionOfKey(key);
    swapPairPositions(position, m_size - 1);
    m_size--;
    
    
    //        DO SOMETHING ABOUT THIS
    //        m_pairs[m_size - 1].key = "";
    //        m_pairs[m_size - 1].value = 0;
    
    return true;
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the map, otherwise
// false.
{
    return (positionOfKey(key) >= 0);
}

bool Map::get(const KeyType& key, ValueType& value) const
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
    if ( ! contains(key))
    {
        return false;
    }
    
    int position = positionOfKey(key);
    value = m_pairs[position].value;
    
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
{
    if ( ! ( i >= 0    &&   i < size()  )  )
    {
        return false;
    }
    
    key = m_pairs[i].key;
    value = m_pairs[i].value;
    
    return true;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
    Map temp = *this;
    *this = other;
    other = temp;
}

//private member function implementations
int Map::positionOfKey(const KeyType& key) const
{
    for (int i = 0; i < m_size; i++)
    {
        if (m_pairs[i].key == key)  //found the key equal to the parameter
        {
            return i;
        }
    }
    
    return -1;  //returns -1 if key is not found
}

void Map::swapPairPositions(int a, int b)
{
    KeyValuePair temp = m_pairs[a];
    m_pairs[a] = m_pairs[b];
    m_pairs[b] = temp;
}
