#include "Actor.h"
#include "StudentWorld.h"


/////////////////////////////////////////
//////ACTOR CLASS IMPLEMENTATIONS////////
/////////////////////////////////////////

//PUBLIC
Actor::Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* studentWorld)
:GraphObject(imageID, startX, startY, dir, size, depth)
{
    m_StudentWorld = studentWorld;
    m_isAlive = true;
}

bool Actor::isAlive() const
{
    return m_isAlive;
}

void Actor::kill()
{
    m_isAlive = false;
}

StudentWorld* Actor::getWorld() const
{
    return m_StudentWorld;
}

bool Actor::isAlien() const
{
    return false;
}

bool Actor::isProjectile() const
{
    return false;
}

void Actor::ifAlienExecuteCollision(int damageAmt)
{
    return;
}

Actor::~Actor()
{
    
}

//PROTECTED
bool Actor::isInBounds(int x, int y) const
{
    return (x >= 0 && x < VIEW_WIDTH && y >= 0 && y < VIEW_HEIGHT);
}

bool Actor::isAliveAndInBounds()
{
    //check to see if currently alive
    if (!isAlive())
    {
        return false;
    }
    
    if (! isInBounds(getX(), getY()))
    {
        kill();
        return false;
    }
    
    return true;
}


////////////////////////////////////////////
///DAMAGEABLEOBJECT CLASS IMPLEMENTATIONS///
////////////////////////////////////////////

//PUBLIC
DamageableObject::DamageableObject(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* studentWorld, double hitPoints)
:Actor(imageID, startX, startY, dir, size, depth, studentWorld)
{
    m_hitPoints = hitPoints;
}

double DamageableObject::hitPoints() const
{
    return m_hitPoints;
}

void DamageableObject::increaseHitPoints(double amt)
{
    m_hitPoints += amt;
    
    if (m_hitPoints > 50)
    {
        m_hitPoints = 50;
    }
}

void DamageableObject::sufferDamage(double amt, int cause)
{
    m_hitPoints -= amt;
}

DamageableObject::~DamageableObject()
{

}


/////////////////////////////////////////
///////STAR CLASS IMPLEMENTATIONS////////
/////////////////////////////////////////

//PUBLIC
Star::Star(StudentWorld* studentWorld, double startX)
:Actor(IID_STAR, startX, randInt(0, VIEW_HEIGHT - 1), 0 , randInt(5, 50) / 100.0,    3,     studentWorld)
//     imageID   startX           startY             dir         size               depth   studentWorld
{
    
}

void Star::doSomething()
{
    //check to see if currently alive
    if (!isAlive())
    {
        return;
    }
    
    //move exactly 1 pixel to the left
    moveTo(getX() - 1, getY());
    
    //check to see if it goes off the grid
    if (getX() <= -1)
    {
        kill();
    }
}

Star::~Star()
{

}


/////////////////////////////////////////
///NACHENBLASTER CLASS IMPLEMENTATIONS///
/////////////////////////////////////////

//PUBLIC
NachenBlaster::NachenBlaster(StudentWorld* studentWorld)
:DamageableObject(IID_NACHENBLASTER,   0,       128,       0,     1.0,     0,    studentWorld,      50)
//                     imageID      startX     startY     dir     size   depth   studentWorld   hitPoints
{
    m_cabbageEnergyPoints = MAXCABBAGEENERGYPOINTS;
    
    m_FlatulenceTorpedoCount = 0;
}

void NachenBlaster::doSomething()
{
    //check to see if currently alive
    if (!isAlive())
    {
        return;
    }
    
    //check to see if the user passed a key
    int ch;
    if (getWorld()->getKey(ch))
    {
        // user hit a key during this tick!
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                if (isInBounds(getX() - 6, getY()))
                {
                    moveTo(getX() - 6, getY());
                }
                break;
                
            case KEY_PRESS_RIGHT:
                if (isInBounds(getX() + 6, getY()))
                {
                    moveTo(getX() + 6, getY());
                }
                break;
                
            case KEY_PRESS_UP:
                if (isInBounds(getX(), getY() + 6))
                {
                    moveTo(getX(), getY() + 6);
                }
                break;
                
            case KEY_PRESS_DOWN:
                if (isInBounds(getX(), getY() - 6))
                {
                    moveTo(getX(), getY() - 6);
                }
                break;
                
            case KEY_PRESS_SPACE:
                if (m_cabbageEnergyPoints >= 5)
                {
                    Cabbage* newCabbage = new Cabbage(getX() + 12, getY(), getWorld());
                    getWorld()->addActor(newCabbage);
                    m_cabbageEnergyPoints -= 5;
                    getWorld()->playSound(SOUND_PLAYER_SHOOT);
                }
                break;
                
            case KEY_PRESS_TAB:
                if (m_FlatulenceTorpedoCount > 0)
                {
                    FlatulenceTorpedo* newTorpedo = new FlatulenceTorpedo(getX() + 12, getY(), 0, getWorld());
                    getWorld()->addActor(newTorpedo);
                    m_FlatulenceTorpedoCount--;
                    getWorld()->playSound(SOUND_TORPEDO);
                }
        }
    }
    
    //Regardless of what the NachenBlaster does, it will receive one cabbage energy point per tick of the game up to a maximum of 30 cabbage energy points
    if (m_cabbageEnergyPoints < MAXCABBAGEENERGYPOINTS)
    {
        m_cabbageEnergyPoints++;
    }
}

void NachenBlaster::increaseFlatulenceTorpedoes(int amt)
{
    m_FlatulenceTorpedoCount += amt;
}

int NachenBlaster::cabbageEnergyPoints() const
{
    return m_cabbageEnergyPoints;
}

int NachenBlaster::flatulenceTorpedoCount() const
{
    return m_FlatulenceTorpedoCount;
}

NachenBlaster::~NachenBlaster()
{

}


//////////////////////////////////////////
/////PROJECTILE CLASS IMPLEMENTATIONS/////
//////////////////////////////////////////

//PUBLIC
Projectile::Projectile(int imageID, double startX, double startY, StudentWorld* studentWorld, int dir)  //dir = 0
:Actor(imageID, startX, startY, dir, .5,     1, studentWorld)
//                                   size  depth
{
    
}

bool Projectile::isProjectile() const
{
    return true;
}

Projectile::~Projectile()
{
    
}

//PROTECTED
bool Projectile::moveAndCollide(int dir, int damage, int numPixels) // = 8 by default
{
    if ( ! isAliveAndInBounds())
    {
        return false;
    }
    
    if (checkForCollision(dir, damage))
    {
        return false;
    }

    moveProjectile(dir, numPixels);
    
    return true;
}

void Projectile::moveProjectile(int dir, int numPixels)
{
    if (dir == RIGHT)
    {
        moveTo(getX() + numPixels, getY());
    }
    else if (dir == LEFT)
    {
        moveTo(getX() - numPixels, getY());
    }
}

bool Projectile::checkForCollision(int dir, int damage)
{
    if (dir == LEFT)    //enemy is NachenBlaster
    {
        NachenBlaster* collidingNachenBlaster = getWorld()->getCollidingNachenBlaster(this);

        if (collidingNachenBlaster != nullptr)  //the turnip collided with the NachenBlaster
        {
            //cause the NachenBlaster to lose damage points
            collidingNachenBlaster->sufferDamage(damage, HIT_BY_PROJECTILE);

            getWorld()->playSound(SOUND_BLAST);

            kill();

            return true;
        }
    }
    else if (dir == RIGHT)  //enemy is aliens
    {
        Actor* collidingAlien = getWorld()->getOneCollidingAlien(this);

        if (collidingAlien != nullptr)  //the projectile collided with an Alien
        {
            collidingAlien->ifAlienExecuteCollision(damage);
            
            kill();
            
            return true;
        }
    }

    return false;
}

void Projectile::rotate()
{
    setDirection(getDirection() + 20);
}


/////////////////////////////////////////
//////CABBAGE CLASS IMPLEMENTATIONS//////
/////////////////////////////////////////

//PUBLIC
Cabbage::Cabbage(double startX, double startY, StudentWorld* studentWorld)
: Projectile(IID_CABBAGE, startX,   startY,  studentWorld)
//             imageID    startX    startY   studentWorld
{
    
}

void Cabbage::doSomething()
{
    if ( ! moveAndCollide(RIGHT, 2))   //has to be right since the cabbage changes directions
    {
        return;
    }

    rotate();
}

Cabbage::~Cabbage()
{

}


/////////////////////////////////////////////
///FLATULENCETORPEDO CLASS IMPLEMENTATIONS///
/////////////////////////////////////////////

//PUBLIC
FlatulenceTorpedo::FlatulenceTorpedo(double startX, double startY, int dir, StudentWorld* studentWorld)
: Projectile(IID_TORPEDO, startX,   startY,   studentWorld, dir)
//             imageID    startX    startY    studentWorld  dir
{
    
}

void FlatulenceTorpedo::doSomething()
{
    if ( ! moveAndCollide(getDirection(), 8))
    {
        return;
    }
}

FlatulenceTorpedo::~FlatulenceTorpedo()
{

}


//////////////////////////////////////////
///////TURNIP CLASS IMPLEMENTATIONS///////
//////////////////////////////////////////

//PUBLIC
Turnip::Turnip(double startX, double startY, StudentWorld* studentWorld)
: Projectile(IID_TURNIP, startX,   startY,   studentWorld)
//            imageID    startX    startY    studentWorld
{
    
}

void Turnip::doSomething()
{
    if ( ! moveAndCollide(LEFT, 2, 6))
    {
        return;
    }
    
    rotate();
}

Turnip::~Turnip()
{

}


/////////////////////////////////////////
//////ALIEN CLASS IMPLEMENTATIONS////////
/////////////////////////////////////////

//PUBLIC
Alien::Alien(int imageID, double startX, double startY, StudentWorld* studentWorld, int flightPlanLength, double travelSpeed, int travelDirection, double hitPoints)
:DamageableObject(imageID, startX, startY,  0,  1.5,    1,   studentWorld,   hitPoints)
//                imageID  startX  startY  dir  size  depth  studentWorld    hitPoints
{
    m_flightPlanLength = flightPlanLength;
    m_travelSpeed = travelSpeed;
    m_travelDirection = travelDirection;
}

void Alien::doSomething()
{
    if ( ! isAliveAndInBounds()) return;
    
    checkForCollision();
    
    if (newFlightPlan())
    {
        //pick a random new flight plan length in the range [1, 32]
        setFlightPlanLength(randInt(1, 32));
    }
    
    if (shootProjectile()) return;
    
    moveAlien();
    
    checkForCollision();
}

int Alien::getFlightPlanLength() const
{
    return m_flightPlanLength;
}

int Alien::getTravelDirection() const
{
    return m_travelDirection;
}

double Alien::getTravelSpeed() const
{
    return m_travelSpeed;
}

void Alien::setFlightPlanLength(int length)
{
    m_flightPlanLength = length;
}

void Alien::setTravelDirection(int direction)
{
    m_travelDirection = direction;
}

void Alien::setTravelSpeed(double speed)
{
    m_travelSpeed = speed;
}

bool Alien::isAlien() const
{
    return true;
}

void Alien::ifAlienExecuteCollision(int damageAmt)
{
    sufferDamage(damageAmt, HIT_BY_PROJECTILE);
    
    if (hitPoints() <= 0)
    {
        increaseScoreForKillingAlien();
        kill();
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->addActor(new Explosion(getX(), getY(), getWorld()));
        dropGoodie();
        
        //add to Aliens killed
        getWorld()->recordAlienDestroyed();
    }
    else
    {
        getWorld()->playSound(SOUND_BLAST);
    }
}

Alien::~Alien()
{

}

//PROTECTED
bool Alien::isAliveAndInBounds()
{
    //check to see if currently alive
    if (!isAlive())
    {
        return false;;
    }
    
    //WAIT I DONT THINK THIS WORKS BECASUE IT CAN HIT THE TOP - you could always run it and freeze one of the coordinates
    //ok check this - so i made it so it only checks the X coordinate
    if (! isInBounds(getX(), 128))
    {
        kill();
        return false;
    }
    
    return true;
}

bool Alien::newFlightPlan()
{
    //check for new flight plan
    if (getFlightPlanLength() == 0 || getY() <= 0 || getY() >= VIEW_HEIGHT - 1)
    {
        if (getY() >= VIEW_HEIGHT - 1)
        {
            setTravelDirection(DOWNANDLEFT);
        }
        else if (getY() <= 0)
        {
            setTravelDirection(UPANDLEFT);
        }
        else if (getFlightPlanLength() == 0)
        {
            //Otherwise if the Alien’s flight plan length is 0, the Alien will set its travel direction by randomly selecting a new one from these three choices: due left, up and left, or down and left
            int randDir = randInt(1, 3);
            
            switch(randDir)
            {
                case 1:
                    setTravelDirection(LEFT);
                    break;
                case 2:
                    setTravelDirection(UPANDLEFT);
                    break;
                case 3:
                    setTravelDirection(DOWNANDLEFT);
                    break;
            }
        }
        
        //this is to make sure that newFlightPlan always returns false for Snagglegons
        if ( getFlightPlanLength() == -1)
        {
            return false;
        }
        
        return true;
    }
    return false;
}

void Alien::moveAlien()
{
    switch (getTravelDirection())
    {
        case UPANDLEFT:
            moveTo(getX() - getTravelSpeed(), getY() + getTravelSpeed());
            break;

        case DOWNANDLEFT:
            moveTo(getX() - getTravelSpeed(), getY() - getTravelSpeed());
            break;

        case LEFT:
            moveTo(getX() - getTravelSpeed(), getY());
            break;
    }
    
    if (getFlightPlanLength() != -1)    //if it's not a Snagglegon
    {
        setFlightPlanLength(getFlightPlanLength() - 1);
    }
}

bool Alien::shootProjectile()
{
    bool didShoot = false;
    
    if (isInPositionToShoot())
    {
        //1 in ((20/CurrentLevelNumber)+5) chance
        int randomInt = randInt(1, (20  / getWorld()->getLevel())  +   5);
        
        if (randomInt == 1) //shoot a Turnip
        {
            Turnip* turnipJustShot = new Turnip(getX() - 14, getY(), getWorld());
            getWorld()->addActor(turnipJustShot);
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
            didShoot = true;    //CHECK THIS
        }
        
        //There is a 1 in ((20/CurrentLevelNumber)+5) chance during this tick that the Smoregon will change its flight plan
        if (canGoRammingSpeed())    //if it's a Smoregon
        {
            int rammingOdds = randInt(1, (20  / getWorld()->getLevel())  +   5);
            
            if (rammingOdds == 1)
            {
                setTravelDirection(LEFT);
                setFlightPlanLength(VIEW_WIDTH);
                setTravelSpeed(5);
            }
        }
    }
    return didShoot;
}

bool Alien::isInPositionToShoot() const
{
    //get NachenBlaster's location
    int NachenBlastersXCoord = getWorld()->getNachenBlaster()->getX();
    int NachenBlastersYCoord = getWorld()->getNachenBlaster()->getY();
//    the NachenBlaster is to the left of the Alien     and   the Alien has a y coordinate that is within [-4,4] pixels of the NachenBlaster’s y coordinate
    return (NachenBlastersXCoord < getX()                &&  (  -4 <= (getY() - NachenBlastersYCoord)     &&     (getY() - NachenBlastersYCoord <= 4) )   );
}

void Alien::checkForCollision()
{
    //check if collided with NachenBlaster
    NachenBlaster* collidingNachenBlaster = getWorld()->getCollidingNachenBlaster(this);
    
    if (collidingNachenBlaster != nullptr)  //the alien collided with the NachenBlaster
    {
        damageNachenBlaster();
        
        kill();
        
        increaseScoreForKillingAlien();
        
        getWorld()->playSound(SOUND_DEATH);
        
        getWorld()->addActor(new Explosion(getX(), getY(), getWorld()));
        
        dropGoodie();
        
        //add to Aliens killed
        getWorld()->recordAlienDestroyed();
    }
}

void Alien::damageNachenBlaster()
{
    //cause the NachenBlaster to lose 5 points
    getWorld()->getNachenBlaster()->sufferDamage(5, HIT_BY_SHIP);
}

void Alien::increaseScoreForKillingAlien()
{
    getWorld()->increaseScore(250);
}

bool Alien::canGoRammingSpeed() const
{
    return false;
}


////////////////////////////////////////////
//////SMALLGON CLASS IMPLEMENTATIONS////////
////////////////////////////////////////////

//PUBLIC
Smallgon::Smallgon(double startX, double startY, StudentWorld* studentWorld)
:Alien(IID_SMALLGON, startX, startY, studentWorld,      0,             2.0,              0,            5 * (1 + (studentWorld->getLevel() - 1) * .1)   )
//                                               flightPlanLength   travelSpeed   travelDirection                         hitPoints
{
   
}

Smallgon::~Smallgon()
{
    
}

//PRIVATE
void Smallgon::dropGoodie()
{
    return;
}


////////////////////////////////////////////
//////SMOREGON CLASS IMPLEMENTATIONS////////
////////////////////////////////////////////

//PUBLIC
Smoregon::Smoregon(double startX, double startY, StudentWorld* studentWorld)
:Alien(IID_SMOREGON, startX, startY, studentWorld,      0,             2.0,              0,             5 * (1 + (studentWorld->getLevel() - 1) * .1)    )
//                                               flightPlanLength   travelSpeed   travelDirection                    hitPoints
{
    
}

Smoregon::~Smoregon()
{
    
}

//PRIVATE
void Smoregon::dropGoodie()
{
    int rand = randInt(1, 3);
    
    if (rand == 1)
    {
        int rand2 = randInt(1, 2);
        
        if (rand2 == 1)
        {
            getWorld()->addActor(new RepairGoodie(getX(), getY(), getWorld()));
        }
        else
        {
            getWorld()->addActor(new FlatulenceTorpedoGoodie(getX(), getY(), getWorld()));
        }
    }
}

bool Smoregon::canGoRammingSpeed() const
{
    return true;
}


////////////////////////////////////////////
//////SNAGGLEGON CLASS IMPLEMENTATIONS//////
////////////////////////////////////////////

//PUBLIC
Snagglegon::Snagglegon(double startX, double startY, StudentWorld* studentWorld)
:Alien(IID_SNAGGLEGON, startX, startY, studentWorld,     -1,            1.75,           DOWNANDLEFT,   10 * (1 + (studentWorld->getLevel() - 1) * .1)    )
//                                               flightPlanLength    travelSpeed   travelDirection          hitPoints
{
    
}

bool Snagglegon::shootProjectile()
{
    if (isInPositionToShoot())
    {
        //1 in ((15/CurrentLevelNumber)+10) chance
        int randomInt = randInt(1, (15  / getWorld()->getLevel())  +   10);
        
        if (randomInt == 1) //shoot a Torpedo
        {
            FlatulenceTorpedo* torpedoJustShot = new FlatulenceTorpedo(getX() - 14, getY(), LEFT, getWorld());
            getWorld()->addActor(torpedoJustShot);
            getWorld()->playSound(SOUND_TORPEDO);
            return true;
        }
    }
    return false;
}

Snagglegon::~Snagglegon()
{
    
}

//PRIVATE
void Snagglegon::dropGoodie()
{
    int rand = randInt(1, 6);
    
    if (rand == 1)
    {
        getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
    }
}

void Snagglegon::damageNachenBlaster()
{
    //cause the NachenBlaster to lose 5 points
    getWorld()->getNachenBlaster()->sufferDamage(15, HIT_BY_SHIP);
}

void Snagglegon::increaseScoreForKillingAlien()
{
    getWorld()->increaseScore(1000);
}


//////////////////////////////////////////
///////GOODIE CLASS IMPLEMENTATIONS///////
//////////////////////////////////////////

//PUBLIC
Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* studentWorld)
:Actor(imageID, startX, startY,  0,   .5,    1,   studentWorld)
//     imageID  startX  startY  dir  size  depth  studentWorld)
{
    
}

void Goodie::doSomething()
{
    if ( ! isAliveAndInBounds())
    {
        return;
    }
    
    if (checkForCollision())
    {
        return;
    }
    
    moveTo(getX() - .75, getY() - .75);
    
    checkForCollision();
}

Goodie::~Goodie()
{
    
}

//PROTECTED
bool Goodie::checkForCollision()
{
    //check if collided with NachenBlaster
    NachenBlaster* collidingNachenBlaster = getWorld()->getCollidingNachenBlaster(this);
    
    if (collidingNachenBlaster != nullptr)  //the goodie collided with the NachenBlaster
    {
        getWorld()->increaseScore(100);
        
        kill();
        
        getWorld()->playSound(SOUND_GOODIE);
        
        giveBenefit();
        
        return true;
    }
    return false;
}


///////////////////////////////////////////////
/////EXTRALIFEGOODIE CLASS IMPLEMENTATIONS/////
///////////////////////////////////////////////

//PUBLIC
ExtraLifeGoodie::ExtraLifeGoodie(double startX, double startY, StudentWorld* studentWorld)
: Goodie(IID_LIFE_GOODIE, startX,   startY, studentWorld)
//           imageID      startX    startY  studentWorld
{
    
}

ExtraLifeGoodie::~ExtraLifeGoodie()
{
    
}

//PRIVATE
void ExtraLifeGoodie::giveBenefit()
{
    getWorld()->incLives();
}


////////////////////////////////////////////
/////REPAIRGOODIE CLASS IMPLEMENTATIONS/////
////////////////////////////////////////////

//PUBLIC
RepairGoodie::RepairGoodie(double startX, double startY, StudentWorld* studentWorld)
: Goodie(IID_REPAIR_GOODIE, startX,   startY,  studentWorld)
//            imageID       startX    startY   studentWorld
{
    
}

RepairGoodie::~RepairGoodie()
{
    
}

//PRIVATE
void RepairGoodie::giveBenefit()
{
    getWorld()->getNachenBlaster()->increaseHitPoints(10);
}


///////////////////////////////////////////////////////
/////FLATULENCETORPEDOGOODIE CLASS IMPLEMENTATIONS/////
///////////////////////////////////////////////////////

//PUBLIC
FlatulenceTorpedoGoodie::FlatulenceTorpedoGoodie(double startX, double startY, StudentWorld* studentWorld)
: Goodie(IID_TORPEDO_GOODIE, startX,   startY,   studentWorld)
//             imageID       startX    startY    studentWorld
{
    
}

FlatulenceTorpedoGoodie::~FlatulenceTorpedoGoodie()
{
    
}

//PRIVATE
void FlatulenceTorpedoGoodie::giveBenefit()
{
    getWorld()->getNachenBlaster()->increaseFlatulenceTorpedoes(5);
}


///////////////////////////////////////////
//////EXPLOSION CLASS IMPLEMENTATIONS//////
///////////////////////////////////////////

//PUBLIC
Explosion::Explosion(double startX, double startY, StudentWorld* studentWorld)
:Actor(IID_EXPLOSION,  startX,    startY,     0,     1,     0,    studentWorld)
//       imageID       startX     startY     dir    size  depth   studentWorld
{
    m_numTicks = 0;
}

void Explosion::doSomething()
{
    //check to see if currently alive
    if (!isAlive())
    {
        return;
    }
    
    if (m_numTicks >= 3)
    {
        kill();
        return;
    }
    
    setSize(getSize() * 1.5);
    m_numTicks++;
}

Explosion::~Explosion()
{

}
