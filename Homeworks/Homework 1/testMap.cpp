//
//  testMap.cpp
//  Map
//
//  Created by Bradley Mont on 1/18/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#include "Map.h"
#include <cassert>
#include <string>
#include <iostream>
using namespace std;


// To test a Map from string to double, leave the following line commented out;
// to test a Map from int to string, remove the "//".

// #define INT_TO_STRING

#ifdef INT_TO_STRING

const KeyType DUMMY_KEY = 9876543;
const ValueType DUMMY_VALUE = "Ouch";
const KeyType NO_SUCH_KEY = 42;
const KeyType KEY1 = 123456789;
const ValueType VALUE1 = "Wow!";

#else // assume string to double

const KeyType DUMMY_KEY = "hello";
const ValueType DUMMY_VALUE = -1234.5;
const KeyType NO_SUCH_KEY = "abc";
const KeyType KEY1 = "xyz";
const ValueType VALUE1 = 9876.5;

#endif

int main()
{
    Map m;
    assert(m.empty());
    ValueType v = DUMMY_VALUE;
    assert( !m.get(NO_SUCH_KEY, v)  &&  v == DUMMY_VALUE); // v unchanged by get failure
    m.insert(KEY1, VALUE1);
    assert(m.size() == 1);
    KeyType k = DUMMY_KEY;
    assert(m.get(0, k, v)  &&  k == KEY1  &&  v == VALUE1);
    cout << "Passed all tests" << endl;
}



/*
 QUESTIONS:
 
 4. By "delete," do I just set the key to "" and the valuw to 0?
 
 
 8. Implementation note 2: The swap function is easily implementable without creating any additional array or additional Map.)
 
 
 */
