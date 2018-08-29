#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

#include <list>

class Actor;
class NachenBlaster;

//constants
const int SMALLGON = 1;
const int SMOREGON = 2;
const int SNAGGLEGON = 3;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    
    virtual ~StudentWorld();
    
    virtual int init();
    
    virtual int move();
    
    virtual void cleanUp();
    
    void addActor(Actor* newActor);
    
    NachenBlaster* getNachenBlaster() const;
    
    // If the NachenBlaster has collided with a, return a pointer to the NachenBlaster;
    // otherwise, return a null pointer.
    NachenBlaster* getCollidingNachenBlaster(const Actor* a) const;
    
    // If there's at least one alien that's collided with a, return
    // a pointer to one of them; otherwise, return a null pointer.
    Actor* getOneCollidingAlien(const Actor* a) const;
    
    Actor* getOneCollidingProjectile(const Actor* a) const;
    
    // Record that one more alien on the current level has been destroyed.
    void recordAlienDestroyed();

private:
    
    //private member variables
    std::list<Actor*> m_actors;
    
    NachenBlaster* m_NachenBlaster;
    
    int m_numDestroyedAlienShips;
    
    int m_necessaryDestroyedAlienShipsToWin;
    
    //private helper functions
    void addStar();
    
    void addAlienShip();
    
    bool theNachenBlasterDiedDuringThisTick();
    
    bool theNachenBlasterCompletedTheCurrentLevel();
    
    void updateDisplayText(); // update the score/lives/level text at screen top
    
    void removeDeadGameObjects();
    
    int numAliensOnScreen() const;
    
    int min(int& a, int& b);
    
    int determineTypeOfAlienShip() const;
    
    bool isWithinEuclidianDistance(const Actor* a, const Actor* b) const;
};

#endif // STUDENTWORLD_H_
