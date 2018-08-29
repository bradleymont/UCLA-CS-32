#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

//im adding this:
#include "Actor.h"
#include <list>

//for the status line at the top
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw


#include <math.h>      //for the square root

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
    m_NachenBlaster = nullptr;
    m_numDestroyedAlienShips = 0;
    m_necessaryDestroyedAlienShipsToWin = 6 + (4 * GameWorld::getLevel());
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    //add stars
    for (int i = 0; i < 30; i++)
    {
        //Star(StudentWorld* studentWorld, double startX)
        Star* starToAdd = new Star(this, randInt(0, VIEW_WIDTH - 1));
        m_actors.push_back(starToAdd);
    }
    
    //add NachenBlaster
    m_NachenBlaster = new NachenBlaster(this);
    
    //i added this
    m_numDestroyedAlienShips = 0;
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    addStar();  //add a new star to the screen with a 1/15 chance
    addAlienShip();
    
    // The term "actors" refers to all aliens, the NachenBlaster, goodies, // stars, explosions, projectiles, stars, etc.
    // Give each actor a chance to do something, incl. the NachenBlaster
    
    //let the NachenBlaster do something
    if (m_NachenBlaster->isAlive())
    {
        m_NachenBlaster->doSomething();
    }
    
    //then let all other Actors do something
    list<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->isAlive())
        {
            // tell each actor to do something (e.g. move)
            (*it)->doSomething();
            
            if (theNachenBlasterDiedDuringThisTick())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if (theNachenBlasterCompletedTheCurrentLevel())
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }
    
    removeDeadGameObjects(); // delete dead game objects
    
    // Update the Game Status Line
    updateDisplayText(); // update the score/lives/level text at screen top
    
    // the player hasn’t completed the current level and hasn’t died, so
    // continue playing the current level
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (m_NachenBlaster != nullptr)
    {
        delete m_NachenBlaster;
        m_NachenBlaster = nullptr;
    }
    
    //remove actors
    list<Actor*>::iterator it = m_actors.begin();
    
    while (it != m_actors.end())
    {
        delete *it;
        it = m_actors.erase(it);
    }
}

void StudentWorld::addActor(Actor* newActor)
{
    m_actors.push_back(newActor);
}

NachenBlaster* StudentWorld::getNachenBlaster() const
{
    return m_NachenBlaster;
}

// If the NachenBlaster has collided with a, return a pointer to the NachenBlaster;
// otherwise, return a null pointer.
NachenBlaster* StudentWorld::getCollidingNachenBlaster(const Actor* a) const
{
    if ( isWithinEuclidianDistance(a, getNachenBlaster())  )
    {
        return getNachenBlaster();
    }
    
    return nullptr;
}

// If there's at least one alien that's collided with a, return
// a pointer to one of them; otherwise, return a null pointer.
Actor* StudentWorld::getOneCollidingAlien(const Actor* a) const
{
    //iterate through the actors
    list<Actor*>::const_iterator it;
    
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->isAlien() && isWithinEuclidianDistance(a, *it))
        {
            return *it;
        }
    }
    
    return nullptr;
}

Actor* StudentWorld::getOneCollidingProjectile(const Actor* a) const
{
    //iterate through the actors
    list<Actor*>::const_iterator it;
    
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->isProjectile() && isWithinEuclidianDistance(a, *it))
        {
            return *it;
        }
    }
    
    return nullptr;
}

void StudentWorld::recordAlienDestroyed()
{
    m_numDestroyedAlienShips++;
}

//private helper functions
void StudentWorld::addStar()
{
    int addStarOdds = randInt(1, 15);
    if (addStarOdds == 1)   //1 in 15 odds
    {
        Star* starToAdd = new Star(this, VIEW_WIDTH - 1);
        m_actors.push_back(starToAdd);
    }
}

void StudentWorld::addAlienShip()
{
    int D = m_numDestroyedAlienShips;
    int T = 6 + (4 * getLevel());
    int R = T - D;
    int M = 4 + (.5 * getLevel());
    
    if (numAliensOnScreen() < min(M, R))
    {
        switch (determineTypeOfAlienShip())
        {
            case SMALLGON:
                addActor(new Smallgon(VIEW_WIDTH - 1,   randInt(0, VIEW_HEIGHT - 1),    this));
                break;
            case SMOREGON:
                addActor(new Smoregon(VIEW_WIDTH - 1,   randInt(0, VIEW_HEIGHT - 1),    this));
                break;
            case SNAGGLEGON:
                addActor(new Snagglegon(VIEW_WIDTH - 1,   randInt(0, VIEW_HEIGHT - 1),  this));
                break;
        }
    }
}

bool StudentWorld::theNachenBlasterDiedDuringThisTick()
{
    return (getNachenBlaster()->hitPoints() <= 0);
}

bool StudentWorld:: theNachenBlasterCompletedTheCurrentLevel()
{
    return (m_numDestroyedAlienShips >= m_necessaryDestroyedAlienShipsToWin);
}

void StudentWorld::updateDisplayText()
{
    if (getNachenBlaster() == nullptr) return;
    
    ostringstream oss;
    
    oss << "Lives: ";
    oss << getLives() << "  ";
    
    oss << "Health: ";
    oss << static_cast<int>((getNachenBlaster()->hitPoints() / INITIALHITPOINTS) * 100);
    oss << "%  ";
    
    oss << "Score: ";
    oss << getScore() << "  ";
    
    oss << "Level: ";
    oss << getLevel() << "  ";
    
    oss << "Cabbages: ";
    oss << static_cast<int>((getNachenBlaster()->cabbageEnergyPoints() / static_cast<double>(MAXCABBAGEENERGYPOINTS)) * 100);
    oss << "%  ";
    
    oss << "Torpedoes: ";
    oss << getNachenBlaster()->flatulenceTorpedoCount();
    
    string result = oss.str();
    setGameStatText(result);
}

void StudentWorld::removeDeadGameObjects()
{
    //remove NachenBlaster (if dead)
    if (m_NachenBlaster != nullptr && ! m_NachenBlaster->isAlive())
    {
        delete m_NachenBlaster;
        m_NachenBlaster = nullptr;
    }
    
    //remove actors (if dead)
    list<Actor*>::iterator it = m_actors.begin();
    
    while (it != m_actors.end())
    {
        if ( ! (*it)->isAlive())    //if the actor is dead
        {
            delete *it;
            it = m_actors.erase(it);
        }
        else
        {
            it++;
        }
    }
}

int StudentWorld::numAliensOnScreen() const
{
    int numAliens = 0;
    
    list<Actor*>::const_iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((*it)->isAlien())
        {
            numAliens++;
        }
    }
    return numAliens;
}

int StudentWorld::min(int& a, int& b)
{
    if (a < b)
    {
        return a;
    }
    return b;
}

int StudentWorld::determineTypeOfAlienShip() const
{
    int S1 = 60;
    int S2 = 20 + getLevel() * 5;
    int S3 = 5 + getLevel() * 10;
    int S = S1 + S2 + S3;
    
    int rand = randInt(1, S);
    
    if (rand <= S1)
    {
        return SMALLGON;
    }
    else if (rand <= S1 + S2)
    {
        return SMOREGON;
    }
    else
    {
        return SNAGGLEGON;
    }
}

bool StudentWorld::isWithinEuclidianDistance(const Actor* a, const Actor* b) const
{
    int X1 = a->getX();
    int Y1 = a->getY();
    int X2 = b->getX();
    int Y2 = b->getY();
    
    int R1 = a->getRadius();
    int R2 = b->getRadius();
    
    int euclidianDist = sqrt(pow(X1 - X2, 2) + pow(Y1 - Y2, 2));
    
    return (euclidianDist < .75 * (R1 + R2));
}
