#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


// ACTOR IMPLEMENTATION

// since all Actor subclasses start w/ direction right (EXCEPTION FLAME BUT DEALT WITH LATER), size 1.0 and alive
Actor::Actor(int imageID, double startX, double startY, int depth, bool flammable, bool infectable, bool blockMove, bool blockFlame, StudentWorld* world)
: GraphObject(imageID, startX, startY, right, depth, 1.0), m_myWorld(world), m_isAlive(true), m_flammable(flammable), m_infectable(infectable), m_blocksMovement(blockMove), m_blocksFlame(blockFlame), m_infectionStatus(false)
{}
Actor::~Actor()
{}

// accessor function for StudentWorld Actor is currently in. I chose to define getWorld() as non-virtual since it is the same for every Actor, and defined it in the Actor class since every subclass of Actor needs this method (and has to access priavte member variable m_myWorld
StudentWorld*Actor::getWorld() const
{
    return m_myWorld;
}
// accessor function for if Actor is currently alive. I chose to define isAlive() as non-virtual since it is the same for every Actor, and defined it in the Actor class since every subclass of Actor needs this method (and has to access priavte member variable m_isAlive) (e.g. in List<Actor*> m_actors)
bool Actor::isAlive() const
{
    return m_isAlive;
}
// makes the Actor dead. I chose to define makeDead() as non-virtual since it is the same for every Actor, and defined it in the Actor class since almost every subclass of Actor needs this method
void Actor::makeDead()
{
    m_isAlive = false;
}
// accessor function for if Actor is flammable. I chose to define isFlammable() as non-virtual since it is the same for every Actor (each Actor subclass will define this in its constructor), and defined it in the Actor class since every subclass of Actor needs this method (and has to access priavte member variable m_isFlammable) (e.g. in List<Actor*> m_actors)
bool Actor::isFlammable() const
{
    return m_flammable;
}
// accessor function for if Actor is infectable. I chose to define isInfectable() as non-virtual since it is the same for every Actor (each Actor subclass will define this in its constructor), and defined it in the Actor class since every subclass of Actor needs this method (and has to access priavte member variable m_isInfectable) (e.g. in List<Actor*> m_actors)
bool Actor::isInfectable() const
{
    return m_infectable;
}
// accessor function for if Actor blocks movement. I chose to define blocksMovement() as non-virtual since it is the same for every Actor (each Actor subclass will define this in its constructor), and defined it in the Actor class since every subclass of Actor needs this method (and has to access priavte member variable m_blocksMovement) (e.g. in List<Actor*> m_actors)
bool Actor::blocksMovement() const
{
    return m_blocksMovement;
}
// accessor function for if Actor blocks flame. I chose to define blocksflame() as non-virtual since it is the same for every Actor (each Actor subclass will define this in its constructor), and defined it in the Actor class since every subclass of Actor needs this method (and has to access priavte member variable m_blocksFlame) (e.g. in List<Actor*> m_actors)
bool Actor::blocksFlame() const
{
    return m_blocksFlame;
}
// Actor will do specified behaviour when comes into contact w/ fire (only called on FLAMMABLE objects). I chose to define burnUp() as virtual as some subclasses of Actor will do different things when they burnUp. I put it in the Actor class because it is called when iterating through m_actors.
void Actor::burnUp()
{
    makeDead();
}
// Accessor function for m_infectionStatus. I chose to define getInfectionStatus as non-virtual since it is the same for every Actor, and defined in the Actor class cuz called when iterating through m_actors.
bool Actor::getInfectionStatus() const
{
    return m_infectionStatus;
}
// Mutator function for m_infectionStatus. I chose to define setInfectionStatus as non-virtual since it is the same for every Actor, and defined in the Actor class cuz called when iterating through m_actors.
void Actor::setInfectionStatus(bool b)
{
    m_infectionStatus = b;
}



// WALL IMPLEMENTATION

// Walls have depth 0, !flammable, !infectable, blocksMovement, blocksFlame
Wall::Wall(double startX, double startY, StudentWorld* world)
: Actor(IID_WALL, startX, startY, 0, false, false, true, true, world)
{}
Wall::~Wall()
{}
// Wall must doSomething() every tick of the game (i.e. nothing). This is virtual because it is redefining the pure virtual func in superclass Actor, specifying the Wall class' specific behaviour.
void Wall::doSomething()
{}



//  ACTIVATING OBJECT IMPLEMENTATION

// every ActivatingObject !infectable and does not blockMove
ActivatingObject::ActivatingObject(int imageID, double startX, double startY, int depth, bool flammable, bool blockFlame,  StudentWorld* world)
: Actor(imageID, startX, startY, depth, flammable, false, false, blockFlame, world), m_active(false)
{}
ActivatingObject::~ActivatingObject()
{}
// accessor function for m_active. I chose to define isActive() as non-virtual since it is the same for every ActivatingObject, and defined in the ActivatingObject class cuz only ActivatingObjects need it.
bool ActivatingObject::isActive() const
{
    return m_active;
}
// mutator function for m_active. I chose to define setActive() as non-virtual since it is the same for every ActivatingObject, and defined in the ActivatingObject class cuz only ActivatingObjects need it.
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
// Exit must doSomething() every tick of the game (defined in spec). This is virtual because it is redefining the pure virtual func in superclass Actor, specifying the Exit class' specific behaviour.
void Exit::doSomething()
{
    // determine if exit overlaps w/ a citizen. if so...
    if(getWorld()->exitOverlapsWithCitizen(getX(), getY()))
    {
        // inc score +500 pts
        getWorld()->increaseScore(500);
        // set Citizen's state to dead (DONE IN exitOverlapsWithCitizen() func)
        getWorld()->playSound(SOUND_CITIZEN_SAVED);
    }
    
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
// Pit must doSomething() every tick of the game (defined in spec). This is virtual because it is redefining the pure virtual func in superclass Actor, specifying the Pit class' specific behaviour.
void Pit::doSomething()
{
    getWorld()->pitExecuteLogic(getX(),getY());
}


// FLAME IMPLEMENTATION

// every Flame depth of 0, !flammable, !blocksFlames
Flame::Flame(double startX, double startY, StudentWorld* world)
: ActivatingObject(IID_FLAME, startX, startY, 0, false, false, world), m_lifeTicks(0)
{}
Flame::~Flame()
{}
// Flame must doSomething() every tick of the game (defined in spec). This is virtual because it is redefining the pure virtual func in superclass Actor, specifying the Flame class' specific behaviour.
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
// Vomit must doSomething() every tick of the game (defined in spec). This is virtual because it is redefining the pure virtual func in superclass Actor, specifying the Vomit class' specific behaviour.
void Vomit::doSomething()
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
    
    // Infect on all infectable objects that overlap w/ vomit
    getWorld()->infectOverlappingItems(getX(), getY());
}


// LANDMINE IMPLEMENTATION

// every Landmine depth of 1, flammable, !blocksFlames, starts w/ 30 safety ticks
Landmine::Landmine(double startX, double startY, StudentWorld* world)
: ActivatingObject(IID_LANDMINE, startX, startY, 1, true, false, world), m_safetyTicks(30)
{}
Landmine::~Landmine()
{}
// Landmine must doSomething() every tick of the game (defined in spec). This is virtual because it is redefining the pure virtual func in superclass Actor, specifying the Landmine class' specific behaviour.
void Landmine::doSomething()
{
    // check if currently alive - if not return immediately
    if(!isAlive())
        return;
    
    // if not yet active then must decrament number safety ticks left.. if safetyticks->0 then becomes active, return immediately
    if(!isActive())
    {
        m_safetyTicks--;
        if(m_safetyTicks <= 0)
        {
            setActive(true);
        }
        return;
    }
    
    // determine if overlaps w/ flammable object (SINCE ACTIVATING OBJECTS WHICH ARE FLAMMABLE DON'T MOVE)
    if(getWorld()->checkOverlapExplodable(getX(), getY(), this))
    {
        // set state to dead
        makeDead();
        explode();
    }
    
}
// when overlaps w/ flame Landmine will explode (full details in spec). This is virtual because it is redefining the virtual func in superclass Actor, specifying what a Landmine specifically will do when overlaps w/ Flame
void Landmine::burnUp()
{
    // set state to dead
    Actor::burnUp();
    explode();
}
// introduce flames in adjacent and same spot and introduce pit same place as landmine was
    // all flames created must have direction up
void Landmine::explode()
{
    // play sound effect that landmine exploded
    getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
    // introduce flame object same location as Landmine
    getWorld()->addFlame(getX(),getY(),up);
    // introduce flame objects around Landmine
    getWorld()->addFlame(getX(),getY()+SPRITE_HEIGHT,up);                      // north
    getWorld()->addFlame(getX()-SPRITE_WIDTH,getY()+SPRITE_HEIGHT,up);         // northeast
    getWorld()->addFlame(getX()-SPRITE_WIDTH,getY(),up);                       // east
    getWorld()->addFlame(getX()-SPRITE_WIDTH,getY()-SPRITE_HEIGHT,up);         // southeast
    getWorld()->addFlame(getX(),getY()-SPRITE_HEIGHT,up);                      // south
    getWorld()->addFlame(getX()+SPRITE_WIDTH,getY()-SPRITE_HEIGHT,up);         // southwest
    getWorld()->addFlame(getX()+SPRITE_WIDTH,getY(),up);                       // west
    getWorld()->addFlame(getX()+SPRITE_WIDTH,getY()+SPRITE_HEIGHT,up);         // northwest
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
// Goodie must doSomething() every tick of the game (defined in spec). This is virtual because it is redefining the pure virtual func in superclass Actor, specifying the Goodie class' specific behaviour. Note: each subclass of Goodie will redefine Goodie's PRIVATE function specificGoodieStuff() which is called within Goodie's doSomething(), thus subclasses of Goodie don't redefine doSomething()
void Goodie::doSomething()
{    
    // must check to see if currently alive - if not then return immediately
    if(!isAlive())
        return;
    
    // determine if overlaps w/ Penelope. if so...
    if(getWorld()->overlaps(getWorld()->getPenelope()->getX(), getWorld()->getPenelope()->getY(), getX(), getY()))
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
: Actor(imageID, startX, startY, 0, true, infectable, true, false, world), m_paralysisTick(0)
{}
Agent::~Agent()
{}
// using box around object see if is blocked from moving in given direction
// checks if Agent is blocked from moving to given position (using method in StudentWorld). I chose to define isBlocked() as non-virtual since it is the same for every Agent, and defined it in the Agent class since only moving Actors (i.e. Agents) will check if they are blocked from moving to a given position
bool Agent::isBlocked(int destX, int destY, Actor* a)
{
    return getWorld()->isBlocked(destX, destY, a);
}
// mutator function for m_paralysisTick. I chose to define incParalysisTick() as non-virtual since it is the same for every Agent, and defined in the Agent class cuz only some subclasses of Agent need it.
void Agent::incParalysisTick()
{
    m_paralysisTick++;
}
// accessor function for if paralyzed (i.e. m_paralysisTick % 2 == 0). I chose to define isParalyzed() as non-virtual since it is the same for every Agent, and defined in the Agent class cuz only some subclasses of Agent need it.
bool Agent::isParalyzed()
{
    // if even paralysis tick then shouldn't do anything
    return (m_paralysisTick % 2 == 0);
}



// ZOMBIE IMPLEMENTATION

// all zombies have Zombie image, !infectable - set movementPlanDist to 0
Zombie::Zombie(double startX, double startY, StudentWorld* world)
: Agent(IID_ZOMBIE, startX, startY, false, world), m_movePlanDist(0)
{}
Zombie::~Zombie()
{}
// Zombie must doSomething() every tick of the game (defined in spec). This is virtual because it is redefining the pure virtual func in superclass Actor, specifying the Zombie class' specific behaviour. Note: each subclass of Zombie will redefine Zombie's PRIVATE function specificZombieStuff() which is called within Zombie's doSomething(), thus subclasses of Zombie don't redefine doSomething(). This function is factored out this way because the majority of what subclasses of Zombie do in doSomething() is very similar.
void Zombie::doSomething()
{
    // check if currently alive - if not then return immediately
    if(!isAlive())
        return;
    
    // paralyzed every even tick - if isParalyzed return immediately
    incParalysisTick();
    if(isParalyzed())
        return;
    
    // check if Person is in front of it in direction is facing. if so...
    Direction dir = getDirection();
    // if below returns true then zombie vomited and should return immediately ... so if false continue
    if(getWorld()->checkPersonInFrontOfZombie(getX(),getY(),dir))
        return;
    
    
    // check if need new movement plan
    if(getMovePlanDist() == 0)
    {
        // pick new random movement plan dist in range 3 thru 10 inclusive
        setMovePlanDist(10-randInt(0,7));
        
        specificZombieStuff();      // dumbZombie and smartZombie do diff things here
    }
    
    
    // then Zombie determine destination coordinate 1 pixel in front of direction facing
    dir = getDirection();
    // below coordinates should be overwritten
    int destX = -1;
    int destY = -1;
    switch(dir)
    {
        case GraphObject::left :
            destX = getX() - 1;
            destY = getY();
            break;
        case GraphObject::right :
            destX = getX() + 1;
            destY = getY();
            break;
        case GraphObject::up :
            destX = getX();
            destY = getY() + 1;
            break;
        case GraphObject::down :
            destX = getX();
            destY = getY() - 1;
            break;
    }
    
    
    // if movement to (destX,destY) not be blocked, move to that location and decrease movement plan
    if(!isBlocked(destX,destY,this))
    {
        moveTo(destX,destY);
        setMovePlanDist(getMovePlanDist() - 1);
    }
    
    // otherwise was blocked from moving, so set movementPlan dist to 0
    else
        setMovePlanDist(0);
}
// accessor function for m_movePlanDist. I chose to define getMovePlanDist() as non-virtual since it is the same for every Zombie, and defined in the Zombie class cuz only subclasses of Zombie need it.
int Zombie::getMovePlanDist() const
{
    return m_movePlanDist;
}
// mutator function for m_movePlanDist. I chose to define setMovePlanDist() as non-virtual since it is the same for every Zombie, and defined in the Zombie class cuz only subclasses of Zombie need it.
void Zombie::setMovePlanDist(int i)
{
    m_movePlanDist = i;
}
// when overlaps w/ Zombie will execute certain behaviour (full details in spec). This is virtual because it is redefining the virtual func in superclass Actor, specifying what a Zombie specifically will do when overlaps w/ Flame. Note: each subclass of Zombie will redefine Zombie's PRIVATE function specificZombieDie() which is called within Zombie's burnUp(), thus subclasses of Zombie don't redefine burnUp(). This function is factored out this way because the majority of what subclasses of Zombie do in burnUp() is very similar.  
void Zombie::burnUp()
{
    // set own state to dead
    Actor::burnUp();
    // play sound effect
    getWorld()->playSound(SOUND_ZOMBIE_DIE);
    
    // increase score by the return value of specificZombieDie (defiend in both smart and dumb zombmies)
    getWorld()->increaseScore(specificZombieDie());
}



// DUMB ZOMBIE IMPLEMENTATION

DumbZombie::DumbZombie(double startX, double startY, StudentWorld* world)
: Zombie(startX, startY, world)
{}
DumbZombie::~DumbZombie()
{}
// STEP 4 doSomething()
void DumbZombie::specificZombieStuff()
{
    // set direction to a random direction
    int rand = randInt(1,4);
    switch(rand)
    {
        case 1:
            setDirection(left);
            break;
        case 2:
            setDirection(right);
            break;
        case 3:
            setDirection(up);
            break;
        case 4:
            setDirection(down);
            break;
    }
}
// returns score for killed zombie, DumbZombie added vaccine drop logic
int DumbZombie::specificZombieDie()
{
    // 1 in 10 chance will drop a vaccine -> if has vaccine goodie will try to fling away
    int chanceVac = randInt(1,10);
    if(chanceVac == 1)      // 10% chance of this
    {
        int destX = getX();
        int destY = getY();
        
        // try to "fling away" Vaccine in random direction -> if would overlap at given position then just return (no vaccine)
        int randDir = randInt(1,4);     // 4 directions
        switch(randDir)
        {
            case 1:
                destX = getX() - SPRITE_WIDTH;
                break;
            case 2:
                destX = getX() + SPRITE_WIDTH;
                break;
            case 3:
                destY = getY() + SPRITE_HEIGHT;
                break;
            case 4:
                destY = getY() - SPRITE_WIDTH;
                break;
        }
        if(!(getWorld()->overlapsWithAnything(destX,destY)))
        {
            getWorld()->addActor(new VaccineGoodie(destX,destY,getWorld()));
        }
    }
    
    
    return 1000;
}



// SMART ZOMBIE IMPLEMENTATION

SmartZombie::SmartZombie(double startX, double startY, StudentWorld* world)
: Zombie(startX, startY, world)
{}
SmartZombie::~SmartZombie()
{}
// STEP 4 doSomething()
void SmartZombie::specificZombieStuff()
{
    // select person closest to smart zombie (smallest Euclidian dist)
    getWorld()->smartZombieLogic(getX(),getY(), this);
}
// returns score for killed zombie, for SmartZombie that's it
int SmartZombie::specificZombieDie()
{
    return 2000;
}


// HUMAN IMPLEMENTATION


// all humans infectable - set infectionStatus and infectionCount to false and 0
Human::Human(int imageID, double startX, double startY, StudentWorld* world)
: Agent(imageID, startX, startY, true, world), m_infectionCount(0)
{}
Human::~Human()
{}
// accessor function for m_infectionCount. I chose to define getInfectionCount() as non-virtual since it is the same for every Human, and defined in the Human class cuz only subclasses of Human need it.
int Human::getInfectionCount() const
{
    return m_infectionCount;
}
// mutator function for m_infectionCount. I chose to define setInfectionCount() as non-virtual since it is the same for every Human, and defined in the Human class cuz only subclasses of Human need it.
void Human::setInfectionCount(int i)
{
    m_infectionCount = i;
}


// CITIZEN IMPLEMENTATION

// every citizen has correct image
Citizen::Citizen(double startX, double startY, StudentWorld* world)
: Human(IID_CITIZEN, startX, startY, world)
{}
Citizen::~Citizen()
{}
// Citizen must doSomething() every tick of the game (defined in spec). This is virtual because it is redefining the pure virtual func in superclass Actor, specifying the Citizen class' specific behaviour.
void Citizen::doSomething()
{
    // check to see if still alive - if not return immediately
    if(!isAlive())
        return;
    
    // check if Citizen infected - if so incInfectCount by 1
    if(getInfectionStatus())
        setInfectionCount(getInfectionCount() + 1);
    // if infection count reaches 500 do following
    if(getInfectionCount() >= 500)
    {
        // must set status to dead
        makeDead();
        // play sound effect
        getWorld()->playSound(SOUND_ZOMBIE_BORN);
        // decrease score -1000
        getWorld()->increaseScore(-1000);
        // introduce new Zombie object to same (x,y) as citizen - 70% chance dumbZombie 30% smartZombie
        int rand = randInt(0,9);
        if(rand >= 3)   // 70% dumbZombie
            getWorld()->addActor(new DumbZombie(getX(),getY(),getWorld()));
        else            // 30% smartZombie
            getWorld()->addActor(new SmartZombie(getX(),getY(),getWorld()));
        // immediately return (since citizen is dead)
        return;
    }
    
    // paralyzed every even tick - if isParalyzed return immediately
    incParalysisTick();
    if(isParalyzed())
        return;
    
    // determine distance to Penelope
    int deltaX = getWorld()->getPenelope()->getX() - getX();
    int deltaY = getWorld()->getPenelope()->getY() - getY();
    int dist_p = sqrt(deltaX*deltaX + deltaY*deltaY);
    
    // determine distance to nearest Zombie
    int dist_z = getWorld()->distNearestZombie(getX(),getY());
    // NOTE: if there are no Zombies in the world will return -1
    
    
    int penelopeX = getWorld()->getPenelope()->getX();
    int penelopeY = getWorld()->getPenelope()->getY();
    
    
    
    
    // PART 6 : if Citizen wants to follow Penelope do following
    
    if( (dist_z == -1 || dist_p < dist_z) && dist_p <= 80 )
    {
        // PART 6.A
        
        // below for part 6.B
        int rand = randInt(0,1);        // if 0 then move vertically (up/down), if 1 then horiz (right/left)
        
        // SAME COL
        if(penelopeX == getX())
        {
            if(penelopeY > getY())      // penelope is above -> MOVE UP
            {
                if(!isBlocked(getX(),getY()+1,this) && !isBlocked(getX(),getY()+2,this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(up);
                    moveTo(getX(),getY()+2);
                    return;         // immediately return
                }
            }
            else                                                // penelope is below -> MOVE DOWN
            {
                if(!isBlocked(getX(),getY()-1,this) && !isBlocked(getX(),getY()-2,this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(down);
                    moveTo(getX(),getY()-2);
                    return;         // immediately return
                }
            }
            // otherwise citizen is blocked -> skip to step 7
        }
        // SAME ROW
        else if(penelopeY == getY())
        {
            if(penelopeX > getX())      // penelope is to right -> MOVE RIGHT
            {
                if(!isBlocked(getX()+1,getY(),this) && !isBlocked(getX()+2,getY(),this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(right);
                    moveTo(getX()+2,getY());
                    return;         // immediately return
                }
            }
            else                                                // penelope is to left -> MOVE LEFT
            {
                if(!isBlocked(getX()-1,getY(),this) && !isBlocked(getX()-2,getY(),this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(left);
                    moveTo(getX()-2,getY());
                    return;         // immediately return
                }
            }
            // otherwise citizen is blocked -> skip to step 7
        }
        
        // PART 6.B
        
        // Penelope TOP RIGHT -> should move up or right
        else if(penelopeY > getY() && penelopeX > getX())
        {
            if(rand == 0)       // MOVE UP
            {
                if(!isBlocked(getX(),getY()+1,this) && !isBlocked(getX(),getY()+2,this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(up);
                    moveTo(getX(),getY()+2);
                    return;         // immediately return
                }
            }
            else if(rand == 1)                // MOVE RIGHT
            {
                if(!isBlocked(getX()+1,getY(),this) && !isBlocked(getX()+2,getY(),this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(right);
                    moveTo(getX()+2,getY());
                    return;         // immediately return
                }
            }
            // if neither of above then was blocked in both directions
        }
        // Penelope TOP LEFT -> should move up or left
        else if(penelopeY > getY() && penelopeX < getX())
        {
            if(rand == 0)       // MOVE UP
            {
                if(!isBlocked(getX(),getY()+1,this) && !isBlocked(getX(),getY()+2,this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(up);
                    moveTo(getX(),getY()+2);
                    return;         // immediately return
                }
            }
            else if(rand == 1)                // MOVE LEFT
            {
                if(!isBlocked(getX()-1,getY(),this) && !isBlocked(getX()-2,getY(),this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(left);
                    moveTo(getX()-2,getY());
                    return;         // immediately return
                }
            }
            // if neither of above then was blocked in both directions
        }
        // Penelope BOTTOM LEFT -> should move down or left
        else if(penelopeY < getY() && penelopeX < getX())
        {
            if(rand == 0)       // MOVE DOWN
            {
                if(!isBlocked(getX(),getY()-1,this) && !isBlocked(getX(),getY()-2,this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(down);
                    moveTo(getX(),getY()-2);
                    return;         // immediately return
                }
            }
            else if(rand == 1)                // MOVE LEFT
            {
                if(!isBlocked(getX()-1,getY(),this) && !isBlocked(getX()-2,getY(),this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(left);
                    moveTo(getX()-2,getY());
                    return;         // immediately return
                }
            }
            // if neither of above then was blocked in both directions
        }
        // Penelope BOTTOM RIGHT -> should move down or right
        else if(penelopeY < getY() && penelopeX > getX())
        {
            if(rand == 0)       // MOVE DOWN
            {
                if(!isBlocked(getX(),getY()-1,this) && !isBlocked(getX(),getY()-2,this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(down);
                    moveTo(getX(),getY()-2);
                    return;         // immediately return
                }
            }
            else if(rand == 1)                // MOVE RIGHT
            {
                if(!isBlocked(getX()+1,getY(),this) && !isBlocked(getX()+2,getY(),this))    // make sure not blocked two pixels in that direction
                {
                    setDirection(right);
                    moveTo(getX()+2,getY());
                    return;         // immediately return
                }
            }
            // if neither of above then was blocked in both directions
        }
    }
    
    // STEP 7 : if Citizen wants to run away from a Zombie
    
    if(dist_z <= 80)
    {
        // for each of four directions check if blocked from moving 2 px that direction. if blocked ignore
        
        int leftDist = -1;
        int rightDist = -1;
        int upDist = -1;
        int downDist = -1;
        // CHECK LEFT (not blocked)
        if(!isBlocked(getX()-1,getY(),this) && !isBlocked(getX()-2,getY(),this))
        {
            leftDist = getWorld()->distNearestZombie(getX()-2,getY());
        }
        // CHECK RIGHT (not blocked)
        if(!isBlocked(getX()+1,getY(),this) && !isBlocked(getX()+2,getY(),this))
        {
            rightDist = getWorld()->distNearestZombie(getX()+2,getY());
        }
        // CHECK UP (not blocked)
        if(!isBlocked(getX(),getY()+1,this) && !isBlocked(getX(),getY()+2,this))
        {
            upDist = getWorld()->distNearestZombie(getX(),getY()+2);
        }
        // CHECK DOWN (not blocked)
        if(!isBlocked(getX(),getY()-1,this) && !isBlocked(getX(),getY()-2,this))
        {
            downDist = getWorld()->distNearestZombie(getX(),getY()-2);
        }

        
        // if none of movement options allow to get further away from zombie then return immediately (no better place to be than where rn)
        if( leftDist <= dist_z && rightDist <= dist_z && upDist <= dist_z && downDist <= dist_z)
        {
            return;
        }
        
        
        // otherwise set Citizen's direction to direction that will take it furthest from nearest zombie, move 2 pixels in that direction, immediately return
        // CHECK LEFT
        if( leftDist != -1 && leftDist > rightDist && leftDist > upDist && leftDist > downDist)
        {
            setDirection(left);
            moveTo(getX()-2,getY());
            return;
        }
        // check RIGHT
        if( rightDist != -1 && rightDist > leftDist && rightDist > upDist && rightDist > downDist)
        {
            setDirection(right);
            moveTo(getX()+2,getY());
            return;
        }
        // check UP
        if( upDist != -1 && upDist > rightDist && upDist > leftDist && upDist > downDist)
        {
            setDirection(up);
            moveTo(getX(),getY()+2);
            return;
        }
        // check DOWN
        if( downDist != -1 && downDist > rightDist && downDist > upDist && downDist > leftDist)
        {
            setDirection(down);
            moveTo(getX(),getY()-2);
            return;
        }
        return;     // immediately return and do nothing more in current tick
    }
    
    // at this point no Zombie whose Euclidian dist from citizen <= 80
    
    else
        return;
    
}

// when overlaps w/ flame Citizen will explode (full details in spec). This is virtual because it is redefining the virtual func in superclass Actor, specifying what a Citizen specifically will do when overlaps w/ Flame
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
    // must check to see if currently alive - if not then return immediately
    if(!isAlive())
        return;
    
    // check if Penelope infected - if so incInfectCount by 1
    if(getInfectionStatus())
        setInfectionCount(getInfectionCount() + 1);
    // if infection count reaches 500 do following
    if(getInfectionCount() >= 500)
    {
        // must set status to dead
        makeDead();
        // play sound effect
        getWorld()->playSound(SOUND_PLAYER_DIE);
        // return immediately - StudentWorld should detect that she's dead and current level end
        return;
    }
    
    // must check if player pressed a key. if so - execute respective key's behaviour
    int key;
    if(getWorld()->getKey(key))
    {
        // must move 4 pixels in given direction (pg 28)
        switch(key)
        {
        // FLAMETHROWER
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
        // LANDMINE
            case KEY_PRESS_TAB:
                // if at least one landmine in inventory
                if(getNumMines() > 0)
                {
                    // decrease num landmines by 1
                    incNumMines(-1);
                    // introduce new landmineObject into game at current (x,y)
                    getWorld()->addActor(new Landmine(getX(), getY(), getWorld()));
                }
                break;
        // VACCINE
            case KEY_PRESS_ENTER :
                // if at least one vaccine in inventory (regardless if infected or not)
                if(getNumVaccines() > 0)
                {
                    // decrease num vaccines by 1
                    incNumVaccines(-1);
                    // set infected status to false and reset InfectionCount
                    setInfectionStatus(false);
                    setInfectionCount(0);
                }
                break;
        // DIRECTION
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(!isBlocked(getX()-4, getY(),this))
                   moveTo(getX()-4,getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(!isBlocked(getX()+4, getY(),this))
                    moveTo(getX()+4,getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if(!isBlocked(getX(), getY()+4,this))
                    moveTo(getX(),getY()+4);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(!isBlocked(getX(), getY()-4,this))
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







