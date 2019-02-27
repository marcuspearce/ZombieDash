#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


// ACTOR member funciton implementations

// since all Actor subclasses start w/ direction right, size 1.0 and alive
Actor::Actor(int imageID, double startX, double startY, int depth, bool flammable, bool infectable, bool blockMove, bool blockFlame, StudentWorld* world)
: GraphObject(imageID, startX, startY, right, depth, 1.0), m_myWorld(world), m_isAlive(true), m_flammable(flammable), m_infectable(infectable), m_blocksMovement(blockMove), m_blocksFlame(blockFlame)
{}
Actor::~Actor()
{}

// need way to access StudentWorld
StudentWorld* Actor::getWorld()
{
    return m_myWorld;
}
//bool Actor::overlaps(int x, int y, Actor&a)
//{
//    return getWorld()->overlaps(x,y,a);
//}
bool Actor::isAlive() const
{
    return m_isAlive;
}
bool Actor::isFlammable() const
{
    return m_flammable;
}
bool Actor::isInfectable() const
{
    return m_infectable;
}
bool Actor::blocksMovement() const
{
    return m_blocksMovement;
}
bool Actor::blocksFlame() const
{
    return m_blocksFlame;
}


// WALL member function implementations

// Walls have depth 0, !flammable, !infectable, blocksMovement, blocksFlame
Wall::Wall(double startX, double startY, StudentWorld* world)
: Actor(IID_WALL, startX, startY, 0, false, false, true, true, world)
{}
Wall::~Wall()
{}
// do nothing, I'm just a wall!
void Wall::doSomething()
{}



//  ACTIVATING OBJECT IMPLEMENTATION

// every ActivatingObject !infectable and does not blockMove
ActivatingObject::ActivatingObject(int imageID, double startX, double startY, int depth, bool flammable, bool blockFlame,  StudentWorld* world)
: Actor(imageID, startX, startY, depth, flammable, false, false, blockFlame, world), m_active(false)
{}
ActivatingObject::~ActivatingObject()
{}
bool ActivatingObject::isActive() const
{
    return m_active;
}
void ActivatingObject::setActive(bool b)
{
    m_active = b;
}


//   EXIT IMPLEMENTATION

// every Exit depth of 1, !flammable, blocksFlames
Exit::Exit(double startX, double startY, StudentWorld* world)
: ActivatingObject(IID_EXIT, startX, startY, 1, false, true, world)
{}
Exit::~Exit()
{}
void Exit::doSomething()
{
    // TODO: IMPLEMENT LATER
}


// PIT IMPLEMENTATION

// every Pit depth of 0, !flammable, !blocksFlames
Pit::Pit(double startX, double startY, StudentWorld* world)
: ActivatingObject(IID_PIT, startX, startY, 0, false, false, world)
{}
Pit::~Pit()
{}
void Pit::doSomething()
{
    // TODO: IMPLEMENT LATER
}


// FLAME IMPLEMENTATION

// every Flame depth of 0, !flammable, !blocksFlames
Flame::Flame(double startX, double startY, StudentWorld* world)
: ActivatingObject(IID_FLAME, startX, startY, 0, false, false, world)
{}
Flame::~Flame()
{}
void Flame::doSomething()
{
    // TODO: IMPLEMENT LATER
}


// VOMIT IMPLEMENTATION

// every Vomit depth of 0, !flammable, !blocksFlames
Vomit::Vomit(double startX, double startY, StudentWorld* world)
: ActivatingObject(IID_VOMIT, startX, startY, 0, false, false, world)
{}
Vomit::~Vomit()
{}
void Vomit::doSomething()
{
    // TODO: IMPLEMENT LATER
}


// LANDMINE IMPLEMENTATION

// every Landmine depth of 1, flammable, !blocksFlames
Landmine::Landmine(double startX, double startY, StudentWorld* world)
: ActivatingObject(IID_LANDMINE, startX, startY, 1, true, false, world)
{}
Landmine::~Landmine()
{}
void Landmine::doSomething()
{
    // TODO: IMPLEMENT LATER
}


// GOODIE IMPLEMENTATION

// every Goodie depth of 1, flammable, !blocksFlames
Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* world)
: ActivatingObject(imageID, startX, startY, 1, true, false, world)
{}
Goodie::~Goodie()
{}
void Goodie::doSomething()
{
    // TODO: IMPLEMENT LATER
    
    // doCommonThingA
    // doDifferentiatedStuff
    // doCommonThingB
}


// VACCINE GOODIE IMPLEMENTATION

// give VaccineGoodie correct image
VaccineGoodie::VaccineGoodie(double startX, double startY, StudentWorld* world)
: Goodie(IID_VACCINE_GOODIE, startX, startY, world)
{}
VaccineGoodie::~VaccineGoodie()
{}
void VaccineGoodie::specificGoodieStuff()
{
    // TODO: IMPLEMENT LATER
}


// GAS CAN GOODIE IMPLEMENTATION

// give GasCanGoodie correct image
GasCanGoodie::GasCanGoodie(double startX, double startY, StudentWorld* world)
: Goodie(IID_GAS_CAN_GOODIE, startX, startY, world)
{}
GasCanGoodie::~GasCanGoodie()
{}
void GasCanGoodie::specificGoodieStuff()
{
    // TODO: IMPLEMENT LATER
}


// LANDMINE GOODIE IMPLEMENTATION

// give LandmineGoodie correct image
LandmineGoodie::LandmineGoodie(double startX, double startY, StudentWorld* world)
: Goodie(IID_LANDMINE_GOODIE, startX, startY, world)
{}
LandmineGoodie::~LandmineGoodie()
{}
void LandmineGoodie::specificGoodieStuff()
{
    // TODO: IMPLEMENT LATER
}





// AGENT IMPLEMENTATION

// All Agents have depth of 0, flammable, blocksMovement, !blocksFlames
Agent::Agent(int imageID, double startX, double startY, bool infectable, StudentWorld* world)
: Actor(imageID, startX, startY, 0, true, infectable, true, false, world)
{}
Agent::~Agent()
{}
// using box around object see if is blocked from moving in given direction
bool Agent::isBlocked(int destX, int destY)
{
    return getWorld()->isBlocked(destX, destY);
}


// ZOMBIE IMPLEMENTATION

// all zombies have Zombie image, !infectable - set movementPlanDist to 0
Zombie::Zombie(double startX, double startY, StudentWorld* world)
: Agent(IID_ZOMBIE, startX, startY, false, world), m_movePlanDist(0)
{}
Zombie::~Zombie()
{}
void Zombie::doSomething()
{
    // TODO: IMPLEMENT LATER
    
    // doCommonThingA
    // doDifferentiatedStuff
    // doCommonThingB
}
int Zombie::getMovePlanDist() const
{
    return m_movePlanDist;
}
void Zombie::setMovePlanDist(int i)
{
    m_movePlanDist = i;
}


// DUMBZOMBIE IMPLEMENTATION

DumbZombie::DumbZombie(double startX, double startY, StudentWorld* world)
: Zombie(startX, startY, world)
{}
DumbZombie::~DumbZombie()
{}
void DumbZombie::specificZombieStuff()
{
    // TODO: IMPLEMENT LATER
}


// SMARTZOMBIE IMPLEMENTATION

SmartZombie::SmartZombie(double startX, double startY, StudentWorld* world)
: Zombie(startX, startY, world)
{}
SmartZombie::~SmartZombie()
{}
void SmartZombie::specificZombieStuff()
{
    // TODO: IMPLEMENT LATER
}


// HUMAN IMPLEMENTATION


// all humans infectable - set infectionStatus and infectionCount to false and 0
Human::Human(int imageID, double startX, double startY, StudentWorld* world)
: Agent(imageID, startX, startY, true, world), m_infectionStatus(false), m_infectionCount(0)
{}
Human::~Human()
{}
bool Human::getInfectionStatus() const
{
    return m_infectionStatus;
}
void Human::setInfectionStatus(bool b)
{
    m_infectionStatus = b;
}
int Human::getInfectionCount() const
{
    return m_infectionCount;
}
void Human::incInfectionCount()
{
    m_infectionCount++;
}


// CITIZEN IMPLEMENTATION

//class Citizen : public Human
//{
//public:
//    Citizen(double startX, double startY, StudentWorld* world);
//    virtual ~Citizen();
//    virtual void doSomething();
//}

// every citizen has correct image
Citizen::Citizen(double startX, double startY, StudentWorld* world)
: Human(IID_CITIZEN, startX, startY, world)
{}
Citizen::~Citizen()
{}
void Citizen::doSomething()
{
    // TODO: IMPLEMENT LATER
}



// PENELOPE member function implementations


Penelope::Penelope(double startX, double startY, StudentWorld* world)
: Human(IID_PLAYER, startX, startY, world), m_numMines(0), m_numFlames(0), m_numVac(0)
{}
Penelope::~Penelope()
{}
void Penelope::doSomething()
{
    // TODO: FINISH IMPLEMENTING 
    
    
    
    if(!isAlive())
        return;
    
    int key;
    if(getWorld()->getKey(key))
    {
        // must move 4 pixels in given direction (pg 28)
        switch(key)
        {
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(!isBlocked(getX()-4, getY()))
                   moveTo(getX()-4,getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(!isBlocked(getX()+4, getY()))
                    moveTo(getX()+4,getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if(!isBlocked(getX(), getY()+4))
                    moveTo(getX(),getY()+4);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(!isBlocked(getX(), getY()-4))
                    moveTo(getX(),getY()-4);
                break;
        }
    }
}





