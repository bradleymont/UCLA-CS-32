//
//  maze.cpp
//  Maze
//
//  Created by Bradley Mont on 2/10/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

//#include <string>
//#include <iostream>
//using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)   return true;
    
    maze[sr][sc] = 'o';
    
    //EAST
    if (maze[sr][sc + 1] == '.')
    {
        if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
            return true;
    }
    
    //SOUTH
    if (maze[sr + 1][sc] == '.')
    {
        if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
            return true;
    }
    
    //WEST
    if (maze[sr][sc - 1] == '.')
    {
        if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
            return true;
    }
    
    //NORTH
    if (maze[sr - 1][sc] == '.')
    {
        if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
            return true;
    }

    return false;
}

//int main()
//{
//    string maze[10] = {
//        "XXXXXXXXXX",
//        "X...X..X.X",
//        "X..XX....X",
//        "X.X.XXXX.X",
//        "XXX......X",
//        "X...X.XX.X",
//        "X.X.X..X.X",
//        "X.XXXX.X.X",
//        "X..X...X.X",
//        "XXXXXXXXXX"
//    };
//
//    if (pathExists(maze, 10,10, 4,3, 1,8))
//        cout << "Solvable!" << endl;
//    else
//        cout << "Out of luck!" << endl;
//}

