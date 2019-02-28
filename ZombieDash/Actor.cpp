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
StudentWorld* Actor::getWorld() const
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
void Actor::makeDead()
{
    m_isAlive = false;
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

// most exits cannot exit (ones that can will overwrite this method)
bool Actor::canExit() const
{
    return false;
}
// this will ONLY BE CALLED ON FLAMMABLE OBJECTS. Some objects will expand on this behaviour
void Actor::burnUp()
{
    makeDead();
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
    // TODO: FINISH CITIZEN PORTION OF INCORPORATING EXIT
    
    // check overlap w/ Citizen and do resulting stuff
    
    // if overlaps w/ Penelope and can exit, inform level Penelope has completed currnet level
    if(getWorld()->penelopeCanExit(getX(), getY()))
        getWorld()->toggleTimeToAdvance();
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
: ActivatingObject(IID_FLAME, startX, startY, 0, false, false, world), m_lifeTicks(0)
{}
Flame::~Flame()
{}
void Flame::doSomething()
{    
    // check to see if currently alive, if not return immediately
    if(!isAlive())
        return;
    
    // after 2 ticks from creation must set own state to dead - return. if not inc lifeTicks by 1
    if(m_lifeTicks >= 2)
    {
        makeDead();
        return;
    }
    m_lifeTicks++;
    
    // damage all damagable objects that overlap w/ flame 
    getWorld()->burnOverlappingItems(getX(), getY());
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
void Landmine::burnUp()
{
    // set state to dead
    Actor::burnUp();
    // play sound effect that landmine exploded
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
    // introduce flame object same location as Landmine
    getWorld()->addFlame(getX(),getY());
    // introduce flame objects around Landmine
    getWorld()->addFlame(getX(),getY()+SPRITE_HEIGHT);                      // north
    getWorld()->addFlame(getX()-SPRITE_WIDTH,getY()+SPRITE_HEIGHT);         // northeast
    getWorld()->addFlame(getX()-SPRITE_WIDTH,getY());                       // east
    getWorld()->addFlame(getX()-SPRITE_WIDTH,getY()-SPRITE_HEIGHT);         // southeast
    getWorld()->addFlame(getX(),getY()-SPRITE_HEIGHT);                      // south
    getWorld()->addFlame(getX()+SPRITE_WIDTH,getY()-SPRITE_HEIGHT);         // southwest
    getWorld()->addFlame(getX()+SPRITE_WIDTH,getY());                       // west
    getWorld()->addFlame(getX()+SPRITE_WIDTH,getY()+SPRITE_HEIGHT);         // northwest
    // introduce Pit at same location as Landmine
    getWorld()->addActor(new Pit(getX(),getY(),getWorld()));
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
    // must check to see if currently alive - if not then return immediately
    if(!isAlive())
        return;
    
    // determine if overlaps w/ Penelope. if so...
    if(getWorld()->overlapsWithPenelope(getX(), getY()))
    {
        // +50 pts
        getWorld()->increaseScore(50);
        // set its state to dead
        makeDead();
        // play sound effect to indicate Penelope picked up a Goodie
        getWorld()->playSound(SOUND_GOT_GOODIE);
        // increment respective inventory depending on Goodie
        specificGoodieStuff();
    }
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
    getWorld()->getPenelope()->incNumVaccines(1);
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
    getWorld()->getPenelope()->incNumFlames(5);
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
    getWorld()->getPenelope()->incNumMines(2);
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
//bool Agent::overlaps(int x, int y)
//{
//    return getWorld()->overlaps(x,y);
//}



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

void Zombie::burnUp()
{
    // set own state to dead
    Actor::burnUp();
    // play sound effect
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
    
    // TODO: FINISH IMPLEMENTING
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
bool Citizen::canExit() const
{
    return true;        // citizens can always exit 
}
void Citizen::burnUp()
{
    // set own state to dead
    Actor::burnUp();
    // play sound effect
    getWorld()->playSound(SOUND_CITIZEN_DIE);
    // decrease player score -1000 pts
    getWorld()->increaseScore(-1000);
}



// PENELOPE member function implementations


Penelope::Penelope(double startX, double startY, StudentWorld* world)
: Human(IID_PLAYER, startX, startY, world), m_numFlames(0), m_numVac(0), m_numMines(0)
{}
Penelope::~Penelope()
{}
void Penelope::doSomething()
{
    // TODO: FINISH IMPLEMENTING 
    
    
    // must check to see if currently alive - if not then return immediately
    if(!isAlive())
        return;
    
    // TODO: CITIZEN INFECTED SHIT
    
    
    int key;
    if(getWorld()->getKey(key))
    {
        // must move 4 pixels in given direction (pg 28)
        switch(key)
        {
            case KEY_PRESS_SPACE:
                // if at least one flame charge in inventory
                if(getNumFlames() > 0)
                {
                    // decrease number flames by 1
                    incNumFlames(-1);
                    // play sound effect
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    // shoot flamethrower (add up to three new objects to the game)
                    getWorld()->flamethrower(getX(), getY());
                }
                break;
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

int Penelope::getNumVaccines() const
{
    return m_numVac;
}
int Penelope::getNumFlames() const
{
    return m_numFlames;
}
int Penelope::getNumMines() const
{
    return m_numMines;
}

void Penelope::incNumVaccines(int n)
{
    m_numVac += n;
}
void Penelope::incNumFlames(int n)
{
    m_numFlames += n;
}
void Penelope::incNumMines(int n)
{
    m_numMines += n;
}
void Penelope::burnUp()
{
    // set own state to dead
    Actor::burnUp();
    // play sound effect
    getWorld()->playSound(SOUND_PLAYER_DIE);
    // StudentWorld object should detect Penelope death and current level ends
}

//// Penelope can only exit when there are no more citizens in the world
//bool Penelope::canExit() const
//{
//    // if no citizens then true Penelope can exit, otherwise false
//    return !(getWorld()->worldContainsCitizens());
//}





