//
//  Flatulan.h
//  ItsAGas
//
//  Created by Bradley Mont on 1/11/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#ifndef Flatulan_h
#define Flatulan_h

class City; //for the City pointer

class Flatulan
{
public:
    // Constructor
    Flatulan(City* cp, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    
    // Mutators
    void move();
    bool possiblyGetConverted();
    
private:
    City* m_city;
    int   m_row;
    int   m_col;
};

#endif /* Flatulan_h */
