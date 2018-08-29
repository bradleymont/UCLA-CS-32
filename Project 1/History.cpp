//
//  History.cpp
//  ItsAGas
//
//  Created by Bradley Mont on 1/12/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#include "History.h"
#include "globals.h"

#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
: m_rows(nRows), m_cols(nCols), hasNumGridBeenInitialized(false)
{
    //all handled by initializer lists
}

bool History::record(int r, int c)
{
    if (   ! (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols)   )
    {
        return false;
    }
    
    if (!hasNumGridBeenInitialized)
    {
        initializeNumGrid();
    }
    
    numGrid[r][c]++;
    return true;
}

void History::display() const
{
    int r, c;
    if (!hasNumGridBeenInitialized)     //print out all dots
    {
        // Draw the grid
        clearScreen();
        for (r = 0; r < m_rows; r++)
        {
            for (c = 0; c < m_cols; c++)
                cout << '.';
            cout << endl;
        }
        cout << endl;
        
        return;
    }
    
    char grid[MAXROWS][MAXCOLS];
    
    for (r = 1; r <= m_rows; r++)
    {
        for (c = 1; c <= m_cols; c++)
        {
            if (numGrid[r][c] == 0)
            {
                grid[r - 1][c - 1] = '.';
            }
            else if (numGrid[r][c] >= 1 && numGrid[r][c] <= 25)
            {
                grid[r - 1][c - 1] = 64 + numGrid[r][c];
            }
            else
            {
                grid[r - 1][c - 1] = 'Z';
            }
        }
    }
    
    // Draw the grid
    clearScreen();
    for (r = 0; r < m_rows; r++)
    {
        for (c = 0; c < m_cols; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
}   //end of display

void History::initializeNumGrid()
{
    for (int r = 1; r <= m_rows; r++)
    {
        for (int c = 1; c <= m_cols; c++)
        {
            numGrid[r][c] = 0;
        }
    }
    hasNumGridBeenInitialized = true;
}
