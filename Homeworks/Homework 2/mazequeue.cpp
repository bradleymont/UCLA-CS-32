//
//  mazequeue.cpp
//  MazeQueue
//
//  Created by Bradley Mont on 2/3/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#include <queue>

#include <string>
#include <iostream>
using namespace std;

const char DISCOVERED = '@';

//(0,0) is the northwest (upper left) corner, with south (down) being the increasing r direction and east (right) being the increasing c direction.
class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
//The parameters are
// - A rectangular maze of Xs and dots that represents the maze. Each string of the array is a row of the maze. Each 'X' represents a wall, and each '.' represents a walkway.
// - The number of rows in the maze.
// - The number of columns in the maze. Each string in the maze parameter must be this length.
// - The starting coordinates in the maze: sr, sc; the row number is in the range 0 through nRows−1, and the column number is in the range 0 through nCols−1.
// - The ending coordinates in the maze: er, ec; the row number is in the range 0 through nRows−1, and the column number is in the range 0 through nCols−1.

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X...X..X.X",
        "X..XX....X",
        "X.X.XXXX.X",
        "XXX......X",
        "X...X.XX.X",
        "X.X.X..X.X",
        "X.XXXX.X.X",
        "X..X...X.X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 4,3, 1,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    queue<Coord> coordQueue;
    
//  Push the starting coordinate (sr,sc) onto the coordinate queue and
//  update maze[sr][sc] to indicate that the algorithm has encountered
//  it (i.e., set maze[sr][sc] to have a value other than '.').
    Coord start(sr, sc);
    coordQueue.push(start);
    maze[sr][sc] = DISCOVERED;
    
//  While the queue is not empty,
//  {
    while (!coordQueue.empty())
    {
//      Pop the front coordinate off the queue. This gives you the current
//      (r,c) location that your algorithm is exploring.
        Coord front = coordQueue.front();
        int currentR = front.r();
        int currentC = front.c();
        coordQueue.pop();
        
//      If the current (r,c) coordinate is equal to the ending coordinate,
//      then we've solved the maze so return true!
        if (currentR == er && currentC == ec)
        {
            return true;
        }
        
//      Check each place you can move from the current cell as follows:
        
//      If you can move EAST and haven't encountered that cell yet,
//      then push the coordinate (r,c+1) onto the queue and update
//      maze[r][c+1] to indicate the algorithm has encountered it.
        if (maze[currentR][currentC + 1] == '.')
        {
            Coord East(currentR, currentC + 1);
            coordQueue.push(East);
            maze[currentR][currentC + 1] = DISCOVERED;
        }
        
//      If you can move SOUTH and haven't encountered that cell yet,
//      then push the coordinate (r+1,c) onto the queue and update
//      maze[r+1][c] to indicate the algorithm has encountered it.
        if (maze[currentR + 1][currentC] == '.')
        {
            Coord South(currentR + 1, currentC);
            coordQueue.push(South);
            maze[currentR + 1][currentC] = DISCOVERED;
        }
        
//      If you can move WEST and haven't encountered that cell yet,
//      then push the coordinate (r,c-1) onto the queue and update
//      maze[r][c-1] to indicate the algorithm has encountered it.
        if (maze[currentR][currentC - 1] == '.')
        {
            Coord West(currentR, currentC - 1);
            coordQueue.push(West);
            maze[currentR][currentC - 1] = DISCOVERED;
        }
        
//      If you can move NORTH and haven't encountered that cell yet,
//      then push the coordinate (r-1,c) onto the queue and update
//      maze[r-1][c] to indicate the algorithm has encountered it.
        if (maze[currentR - 1][currentC] == '.')
        {
            Coord North(currentR - 1, currentC);
            coordQueue.push(North);
            maze[currentR - 1][currentC] = DISCOVERED;
        }
    }   //end of the while loop
    
//  There was no solution, so return false
    return false;
}
