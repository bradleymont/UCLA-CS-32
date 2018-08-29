//
//  Game.h
//  ItsAGas
//
//  Created by Bradley Mont on 1/11/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#ifndef Game_h
#define Game_h

class City;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nFlatulans);
    ~Game();
    
    // Mutators
    void play();
    
private:
    City* m_city;
};

#endif /* Game_h */
