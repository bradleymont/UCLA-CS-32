//
//  History.h
//  ItsAGas
//
//  Created by Bradley Mont on 1/12/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#ifndef History_h
#define History_h

#include "globals.h"

class History
{
public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
    
private:
    int m_rows;
    int m_cols;
    int numGrid[MAXROWS][MAXCOLS];
    void initializeNumGrid();
    bool hasNumGridBeenInitialized;
};

#endif /* History_h */
