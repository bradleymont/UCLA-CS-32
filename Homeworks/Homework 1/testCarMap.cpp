//
//  testCarMap.cpp
//  CarMap
//
//  Created by Bradley Mont on 1/20/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#include "CarMap.h"

#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    CarMap c;
    
    assert(c.fleetSize() == 0);
    
    assert(c.addCar("A"));
    assert(c.addCar("B"));
    assert(c.addCar("C"));
    assert(c.addCar("D"));
    
    assert(!c.addCar("A"));
    
    assert(c.fleetSize() == 4);
    
    assert(c.gas("A") == 0);
    assert(c.gas("aA") == -1);
    
    assert(c.addGas("A", 10));
    assert(c.addGas("B", 20));
    assert(c.addGas("C", 30));
    assert(c.addGas("D", 40));
    
    assert(!c.addGas("Aa", 10));
    assert(!c.addGas("A", -10));
    assert(!c.addGas("Aa", -10));
    
    assert(c.gas("A") == 10);
    assert(c.gas("B") == 20);
    assert(c.gas("C") == 30);
    assert(c.gas("D") == 40);
    
    assert(c.useGas("A", 5));
    assert(c.useGas("B", 10));
    assert(c.useGas("C", 15));
    assert(c.useGas("D", 20));
    
    assert(!c.useGas("Aa", 10));
    assert(!c.useGas("A", -10));
    assert(!c.useGas("Aa", 6));
    
    cout << "All test cases passed! Now let's print:" << endl;
    
    c.print();
}
