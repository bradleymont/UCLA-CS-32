//
//  Map.cpp
//
//  Created by Bradley Mont on 1/26/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#include "Map.h"

Map::Map()           // Create an empty map (i.e., one with no key/value pairs)
{
    m_size = 0;
    head = nullptr;
}

Map::Map(const Map& other)  //copy constructor
{
    m_size = 0;     //insert will increment m_size to the correct size
    head = nullptr;
    Node* otherP = other.head;
    while (otherP != nullptr)   //iterate through all of other
    {
        insert(otherP->m_key, otherP->m_value);    //copy over each of other's nodes
        otherP = otherP->next;
    }
}

Map::~Map()     //destructor
{
    deleteAllNodes();
}

Map& Map::operator=(const Map& rhs)     //assignment operator
{
    if (&rhs == this)   //they're the same variable
    {
        return (*this);
    }
    
    //delete current object's linked list
    deleteAllNodes();
    head = nullptr;

    m_size = 0; //insert increments it

    Node* otherP = rhs.head;
    
    while (otherP != nullptr)
    {
        insert(otherP->m_key, otherP->m_value);
        otherP = otherP->next;
    }
    
    return (*this);
}

bool Map::empty() const   // Return true if the map is empty, otherwise false.
{
    return (head == nullptr);
}

int Map::size() const     // Return the number of key/value pairs in the map.
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full.
{
    if (contains(key))  //key is equal to a key already in the map
    {
        return false;
    }
    
    if (empty())
    {
        addToEmpty(key, value);
        return true;
    }
    
    //Add to the rear
    
    Node* p;
    p = head;   //start at top node
    
    //the loop continues until p points at the last node in the list
    while (p->next != nullptr)
    {
        p = p->next;
    }
    
    //create the node to be added
    Node* n = new Node;
    n->m_key = key;
    n->m_value = value;
    
    //p (now the 2nd to last item) should now have next point to the new node
    p->next = n;
    
    //n's next should be nullptr since it's the last node
    //prev should be p, the element before it
    n->next = nullptr;
    n->prev = p;
    
    m_size++;
    
    return true;
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
    
    Node* p = head;
    
    //iterate through the whole list
    while (p != nullptr)    //CHECK THIS
    {
        if (p->m_key == key)    //found the key in the map equal to key
        {
            p->m_value = value;
            break;
        }
        p = p->next;
    }
    
    return true;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full.
{
    if (insert(key, value)) //returns true if contains() == false
    {
        return true;
    }
    else    //contains() == true
    {
        (update(key, value));
        return true;
    }
}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
{
    if (!contains(key)) //will return false if the list is empty (head = nullptr)
    {
        return false;
    }
    
    //CASE #1: we're erasing the first node
    if(head->m_key == key)
    {
        Node* toBeDeleted = head;
        head = toBeDeleted->next;
        delete toBeDeleted;
        m_size--;
        return true;
    }
    
    //CASE #2: we're deleting an interior node or the last node
    Node* p = head;
    
    while (p != nullptr)
    {
        if (p->next != nullptr && p->next->m_key == key)
        {
            break;  //p points to the node above
        }
        
        p = p->next;
    }
    
    if (p != nullptr)   //value found
    {
        Node* toBeDeleted = p->next;
        p->next = toBeDeleted->next;    //update next pointer of the previous element
        
        if (toBeDeleted->next != nullptr)   //if toBeDeleted isn't the last element
        {
            toBeDeleted->next->prev = p;    //update prev pointer of the next element
        }
        delete toBeDeleted;
        m_size--;
    }
    
    return true;
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the map, otherwise
// false.
{
    Node* p = head;
    
    while (p != nullptr)
    {
        if (p->m_key == key)
        {
            return true;
        }
        
        p = p->next;
    }
    
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
    if (!contains(key))
    {
        return false;
    }
    
    Node* p = head;
    
    while (p != nullptr)
    {
        if (p->m_key == key)
        {
            value = p->m_value;
            break;
        }
        p = p->next;
    }
    
    return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.
{
    if ( !  ( 0 <= i  &&  i < size() )   )
    {
        return false;
    }
    
    int counter = 0;
    Node* p = head;
    
    while (p != nullptr)
    {
        if (counter == i)
        {
            key = p->m_key;
            value = p->m_value;
            break;
        }
        
        p = p->next;
        counter++;
    }
    
    return true;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
    //check for aliasing
    if (this == &other)
    {
        return;
    }

    //swap head pointers
    Node* tempHead = head;
    head = other.head;
    other.head = tempHead;

    //swap sizes
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
}

//private member functions
void Map::addToEmpty(const KeyType& key, const ValueType& value)
{
    Node* p;
    p = new Node;
    p->m_key = key;
    p->m_value = value;
    p->next = nullptr;
    p->prev = nullptr;
    
    head = p;
    m_size++;
}

void Map::deleteAllNodes()
{
    Node* p;
    p = head;
    
    while(p != nullptr)
    {
        Node* n = p->next;
        delete p;
        p = n;
    }
}

//non-member functions
bool combine(const Map& m1, const Map& m2, Map& result)
{
     Map tempResult;
     
     KeyType currentKey;
     ValueType currentValue = 0;
     
     //iterate through m1 and copy everything
     for (int i = 0; i < m1.size(); i++)
     {
         m1.get(i, currentKey, currentValue);
         tempResult.insert(currentKey, currentValue);
     }
     
     //iterate through m2 and be careful of duplicates
     for (int i = 0; i < m2.size(); i++)
     {
         m2.get(i, currentKey, currentValue);
         
         if (! tempResult.contains(currentKey))  //if both Maps don't have that value
         {
             tempResult.insert(currentKey, currentValue);
         }
         else  // both maps have the same value
         {
             ValueType otherValue;
             m1.get(currentKey, otherValue);
             
             if (currentValue != otherValue)
             {
                 return false;
             }
         }
     }
     
     result = tempResult;
     
     return true;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
    Map tempResult;
    
    KeyType currentKey;
    ValueType currentValue = 0;
    
    //iterate through m1
    for (int i = 0; i < m1.size(); i++)
    {
        m1.get(i, currentKey, currentValue);
        
        if ( ! m2.contains(currentKey)) //if both Maps don't have that value
        {
            tempResult.insert(currentKey, currentValue);
        }
    }
    
    result = tempResult;
}
