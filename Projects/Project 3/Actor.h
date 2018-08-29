#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

//CONSTANTS
const int MAXCABBAGEENERGYPOINTS = 30;
const int INITIALHITPOINTS = 50;
const int RIGHT = 0;
const int LEFT = 180;
const int DOWNANDLEFT = 225;
const int UPANDLEFT = 135;
const int HIT_BY_SHIP = 0;
const int HIT_BY_PROJECTILE = 1;

///////////////////////////////////
//////ACTOR CLASS DECLARATION//////
///////////////////////////////////

class Actor: public GraphObject
{
public:
    
    Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* studentWorld);
    
    virtual void doSomething() = 0;
    
    bool isAlive() const;
    
    void kill();
    
    StudentWorld* getWorld() const;
    
    virtual bool isAlien() const;
    
    virtual bool isProjectile() const;
    
    virtual void ifAlienExecuteCollision(int damageAmt);
    
    virtual ~Actor();
    
protected:
    
    bool isInBounds(int x, int y) const;
    
    virtual bool isAliveAndInBounds();
    
private:
    
    StudentWorld* m_StudentWorld;
    
    bool m_isAlive;
};


//////////////////////////////////////////////
//////DAMAGEABLEOBJECT CLASS DECLARATION//////
//////////////////////////////////////////////


class DamageableObject: public Actor
{
public:
    
    DamageableObject(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* studentWorld, double hitPoints);
    
    double hitPoints() const;
    
    void increaseHitPoints(double amt);
    
    // This actor suffers an amount of damage caused by being hit by either
    // a ship or a projectile (see constants above).
    virtual void sufferDamage(double amt, int cause);
    
    virtual ~DamageableObject();
    
private:
    
    double m_hitPoints;
};


////////////////////////////////////
///////STAR CLASS DECLARATION///////
////////////////////////////////////


class Star: public Actor
{
public:
    
    Star(StudentWorld* studentWorld, double startX);
    
    virtual void doSomething();
    
    virtual ~Star();
};


///////////////////////////////////////////
//////NACHENBLASTER CLASS DECLARATION//////
///////////////////////////////////////////


class NachenBlaster: public DamageableObject
{
public:
    
    NachenBlaster(StudentWorld* studentWorld);
    
    virtual void doSomething();
    
    void increaseFlatulenceTorpedoes(int amt);
    
    int cabbageEnergyPoints() const;
    
    int flatulenceTorpedoCount() const;
    
    virtual ~NachenBlaster();
    
private:
    
    int m_cabbageEnergyPoints;
    
    int m_FlatulenceTorpedoCount;
};


////////////////////////////////////
////PROJECTILE CLASS DECLARATION////
////////////////////////////////////


class Projectile: public Actor
{
public:

    Projectile(int imageID, double startX, double startY, StudentWorld* studentWorld, int dir = RIGHT);
    
    virtual bool isProjectile() const;
    
    virtual ~Projectile();
    
protected:
    
    virtual bool moveAndCollide(int dir, int damage, int numPixels = 8);   //returns true if the Projectile succesfully moved
    
    void moveProjectile(int dir, int numPixels);
    
    virtual bool checkForCollision(int dir, int damage = 2);
    
    void rotate();
};


/////////////////////////////////////
//////CABBAGE CLASS DECLARATION//////
/////////////////////////////////////


class Cabbage: public Projectile
{
public:
    
    Cabbage(double startX, double startY, StudentWorld* studentWorld);
    
    virtual void doSomething();
    
    virtual ~Cabbage();
};


///////////////////////////////////////////////
//////FLATULENCETORPEDO CLASS DECLARATION//////
///////////////////////////////////////////////


class FlatulenceTorpedo: public Projectile
{
public:
    
    FlatulenceTorpedo(double startX, double startY, int dir, StudentWorld* studentWorld);
    
    virtual void doSomething();
    
    virtual ~FlatulenceTorpedo();
};


////////////////////////////////////
//////TURNIP CLASS DECLARATION//////
////////////////////////////////////


class Turnip: public Projectile
{
public:
    Turnip(double startX, double startY, StudentWorld* studentWorld);
    
    virtual void doSomething();
    
    virtual ~Turnip();
};

/////////////////////////////////////
///////ALIEN CLASS DECLARATION///////
/////////////////////////////////////

class Alien: public DamageableObject
{
public:
    
    Alien(int imageID, double startX, double startY, StudentWorld* studentWorld, int flightPlanLength, double travelSpeed, int travelDirection, double hitPoints);
    
    virtual void doSomething();     //all Aliens run the same doSomething
    
    //getters
    int getFlightPlanLength() const;
    
    int getTravelDirection() const;
    
    double getTravelSpeed() const;
    
    //setters
    void setFlightPlanLength(int length);
    
    void setTravelDirection(int direction);
    
    void setTravelSpeed(double speed);
    
    //other
    virtual bool isAlien() const;
    
    virtual void ifAlienExecuteCollision(int damageAmt);
    
    virtual ~Alien();
 
protected:
    
    virtual bool isAliveAndInBounds();  //has to be overriden since it can't check for top and bottom (not const since can kill())
    
    bool newFlightPlan();
    
    void moveAlien();
    
    virtual bool shootProjectile();
    
    bool isInPositionToShoot() const;
    
    //collisions
    void checkForCollision();
    
    virtual void damageNachenBlaster();
    
    virtual void increaseScoreForKillingAlien();
    
    virtual bool canGoRammingSpeed() const;
    
    virtual void dropGoodie() = 0;
    
private:
    
    int m_flightPlanLength;
    
    double m_travelSpeed;
    
    int m_travelDirection;
};


//////////////////////////////////////
//////SMALLGON CLASS DECLARATION//////
//////////////////////////////////////


class Smallgon: public Alien
{
public:
    
    Smallgon(double startX, double startY, StudentWorld* studentWorld);
    
    virtual ~Smallgon();
    
private:
    
    virtual void dropGoodie();
};

//////////////////////////////////////
//////SMOREGON CLASS DECLARATION//////
//////////////////////////////////////


class Smoregon: public Alien
{
public:
    
    Smoregon(double startX, double startY, StudentWorld* studentWorld);
    
    virtual ~Smoregon();
    
private:
    
    virtual void dropGoodie();
    
    virtual bool canGoRammingSpeed() const;
};


////////////////////////////////////////
//////SNAGGLEGON CLASS DECLARATION//////
////////////////////////////////////////


class Snagglegon: public Alien
{
public:
    
    Snagglegon(double startX, double startY, StudentWorld* studentWorld);
    
    virtual bool shootProjectile();
    
    virtual ~Snagglegon();
    
private:
    
    virtual void dropGoodie();
    
    virtual void damageNachenBlaster();
    
    virtual void increaseScoreForKillingAlien();
};


////////////////////////////////////
//////GOODIE CLASS DECLARATION//////
////////////////////////////////////


class Goodie: public Actor
{
public:
    
    Goodie(int imageID, double startX, double startY, StudentWorld* studentWorld);
    
    virtual void doSomething();
    
    virtual ~Goodie();
    
protected:
    
    bool checkForCollision();
    
    virtual void giveBenefit() = 0;
};

/////////////////////////////////////////////
//////EXTRALIFEGOODIE CLASS DECLARATION//////
/////////////////////////////////////////////

class ExtraLifeGoodie: public Goodie
{
public:
    
    ExtraLifeGoodie(double startX, double startY, StudentWorld* studentWorld);
    
    virtual ~ExtraLifeGoodie();
    
private:
    
    virtual void giveBenefit();
};

//////////////////////////////////////////
//////REPAIRGOODIE CLASS DECLARATION//////
//////////////////////////////////////////

class RepairGoodie: public Goodie
{
public:
    
    RepairGoodie(double startX, double startY, StudentWorld* studentWorld);
    
    virtual ~RepairGoodie();
    
private:
    
    virtual void giveBenefit();
};

/////////////////////////////////////////////////////
//////FLATULENCETORPEDOGOODIE CLASS DECLARATION//////
/////////////////////////////////////////////////////

class FlatulenceTorpedoGoodie: public Goodie
{
public:
    
    FlatulenceTorpedoGoodie(double startX, double startY, StudentWorld* studentWorld);
    
    virtual ~FlatulenceTorpedoGoodie();
    
private:
    
    virtual void giveBenefit();
};


///////////////////////////////////////
//////EXPLOSION CLASS DECLARATION//////
///////////////////////////////////////


class Explosion: public Actor
{
public:
    
    Explosion(double startX, double startY, StudentWorld* studentWorld);
    
    virtual void doSomething();
    
    virtual ~Explosion();
    
private:
    
    int m_numTicks;
};

#endif // ACTOR_H_
