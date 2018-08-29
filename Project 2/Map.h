//
//  Map.h
//  DoubleTrouble
//
//  Created by Bradley Mont on 1/26/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <string>

using KeyType = std::string;
using ValueType = double;

//for testing other KeyTypes and ValueTypes
//using KeyType = int;
//using ValueType = std::string;

class Map
{
public:
    Map();      // Create an empty map (i.e., one with no key/value pairs)
    
    Map(const Map& other);  //copy constructor
    
    ~Map(); //destructor
    
    Map& operator=(const Map& rhs); //assignment operator
    
    bool empty() const;  // Return true if the map is empty, otherwise false.
    
    int size() const;    // Return the number of key/value pairs in the map.
    
    bool insert(const KeyType& key, const ValueType& value);
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false.
    
    bool update(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // Otherwise, make no change to the map and return false.
    
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value it currently maps to, but instead map to
    // the value of the second parameter; return true in this case.
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false.
    
    bool erase(const KeyType& key);
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    
    bool contains(const KeyType& key) const;
    // Return true if key is equal to a key currently in the map, otherwise
    // false.
    
    bool get(const KeyType& key, ValueType& value) const;
    // If key is equal to a key currently in the map, set value to the
    // value in the map that that key maps to and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    
    bool get(int i, KeyType& key, ValueType& value) const;
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of one of the key/value pairs in the map and return
    // true.  Otherwise, leave the key and value parameters unchanged and
    // return false.
    
    void swap(Map& other);
    // Exchange the contents of this map with the other one.
    
private:
    // Since this structure is used only by the implementation of the
    // Map class, we'll make it private to Map.
    struct Node
    {
        KeyType   m_key;
        ValueType m_value;
        
        Node* next;
        Node* prev;
    };
    
    //private member variables
    Node* head;
    int m_size;
    
    //private member functions
    void addToEmpty(const KeyType& key, const ValueType& value);
    void deleteAllNodes();
};

//non-member function prototypes
bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif /* Map_h */
