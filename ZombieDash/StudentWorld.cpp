#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <sstream>
#include <list>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_timeToAdvance(false)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}


// Allocates and inserts all Actors into GameWorld as specified in current level's data file
int StudentWorld::init()
{
    Level lev(assetPath());
    
    // how to choose which level to load
    ostringstream oss;
    oss << getLevel();  // since this returns int easier to use ostringstream
    string levelFile = "level0" + oss.str() + ".txt";
    //    string levelFile = "level02.txt";
    
    // error checking
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (getLevel() > 99 || result == Level::load_fail_file_not_found)
        return GWSTATUS_PLAYER_WON;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        
        for(int i = 0; i < 16; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                Level::MazeEntry me = lev.getContentsOf(i,j);
                
                // allocate and insert all Actors into GameWorld depending on symbol loaded in data file
                switch(me)
                {
                    case Level::wall :
                        m_actors.push_back(new Wall(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::exit :
                        m_actors.push_back(new Exit(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::pit :
                        m_actors.push_back(new Pit(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::vaccine_goodie :
                        m_actors.push_back(new VaccineGoodie(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::gas_can_goodie :
                        m_actors.push_back(new GasCanGoodie(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::landmine_goodie :
                        m_actors.push_back(new LandmineGoodie(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::dumb_zombie :
                        m_actors.push_back(new DumbZombie(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::smart_zombie :
                        m_actors.push_back(new SmartZombie(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::citizen :
                        m_actors.push_back(new Citizen(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this));
                        break;
                    case Level::player :
                        m_penelope = new Penelope(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, this);
                        break;
                    default:
                        break;      // if none of above then just empty space
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}



// called every tick (roughly 20 times per second) - ask each Actor to doSomething, delete any Actors that have died during tick, update statusText
int StudentWorld::move()
{
    // ASK ALL ACTORS TO DO SOMETHING IF THEY ARE ALIVE
    
    // first Penelope should doSomething
    if(m_penelope->isAlive())
        m_penelope->doSomething();
    
    // iterate thru list of Actors and each of them doSomething if they're alive
    list<Actor*>::iterator it;
    it = m_actors.begin();
    while(it != m_actors.end())
    {
        if((*it)->isAlive())
            (*it)->doSomething();
        
        // if Actor does smt that causes Penelope to die, method immediately returns GWSTATUS_PLAYER_DIED
        if(!(m_penelope->isAlive()))
        {
            decLives();     // must decrement number lives after dies
            increaseScore(-1000);
            return GWSTATUS_PLAYER_DIED;
        }
        
        // if given Actor died on current turn, remove it (ADDED TO FIX PIT AND LANDMINE SAME LOCATION
        if(!((*it)->isAlive()))     // if given Actor is dead
        {
            delete(*it);
            it = m_actors.erase(it);    // not a Vector so this is ok, returns iterator pointing after erased
        }
        
        // if all remaining Citizens and Penelope have used exit and time to advance to new level, return GWSTATUS_FINISHED_LEVEL
            // this is done in the doSomething() of Exit object
        
        
        if(m_timeToAdvance)
        {
            // play finished level file
            playSound(SOUND_LEVEL_FINISHED);
            // change back to false for next level
            toggleTimeToAdvance();
            return GWSTATUS_FINISHED_LEVEL;
        }
        
        it++;
    }
    

    // DELETE ANY ACTORS THAT HAVE DIED DURING THIS TICK AND REMOVE THEM FROM COLLECTION OF ACTORS
    
    it = m_actors.begin();      // reuse iterator from before
    // iterate thru all Actors and delete the dead ones
    while(it != m_actors.end())
    {
        if(!((*it)->isAlive()))     // if given Actor is dead
        {
            delete(*it);
            it = m_actors.erase(it);    // not a Vector so this is ok, returns iterator pointing after erased
        }
        else
            it++;
    }
    
    
    // UPDATE STATUS TEXT ON TOP OF SCREEN W/ LATEST INFO
    
    // make score always have 6 characters
    ostringstream score;
    if(getScore() < 0)
    {
        score.fill('0');
        // necessary because otherwise '-' may go in the middle of the score -> multiply by -1 because otherwise two '-' in
        score << '-' << setw(5) << -1*getScore();       
    }
    else
    {
        score.fill('0');
        score << setw(6) << getScore();
    }
    
    // use stringStream cuz can convert int type inputs into str easily
    ostringstream statText;
    statText << "Score: " << score.str() << "  Level: " << getLevel() << "  Lives: " << getLives() << "  Vacc: " << m_penelope->getNumVaccines() << "  Flames: " << m_penelope->getNumFlames() << "  Mines: " << m_penelope->getNumMines() << "  Infected: " << m_penelope->getInfectionCount();
    
    setGameStatText(statText.str());
    
    
    // if none of above conditions true return GWSTATUS_CONTINUE_GAME
    
    return GWSTATUS_CONTINUE_GAME;
}



void StudentWorld::cleanUp()
{
    // make sure two consecutive calls to cleanUp doesn't do anything undefined
    if(m_penelope != nullptr)
    {
        delete m_penelope;
        m_penelope = nullptr;
    }
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();

    // delete all actors
    while(it != m_actors.end())
    {
        delete(*it);
        it = m_actors.erase(it);
    }
}




Penelope* StudentWorld::getPenelope()
{
    return m_penelope;
}

void StudentWorld::addActor(Actor* a) 
{
    m_actors.push_back(a);
}




// see if object should BLOCK movment (pg 10 spec)
bool StudentWorld::isBlocked(int x, int y, Actor* a)
{
    // check Penelope (since not in m_actors)
    if(m_penelope != a)
    {
        // check bottom left corner,
        if( (x >= m_penelope->getX() && x <= m_penelope->getX()+SPRITE_WIDTH-1) && (y >= m_penelope->getY() && y <= m_penelope->getY()+SPRITE_HEIGHT-1) )
            return true;
        // check top left corner
        if ( (x >= m_penelope->getX() && x <= m_penelope->getX()+SPRITE_WIDTH-1) && (y+SPRITE_HEIGHT-1 >= m_penelope->getY() && y+SPRITE_HEIGHT-1 <= m_penelope->getY()+SPRITE_HEIGHT-1) )
            return true;
        // check bottom right corner
        if( (x+SPRITE_WIDTH-1 >= m_penelope->getX() && x+SPRITE_WIDTH-1 <= m_penelope->getX()+SPRITE_WIDTH-1) && (y >= m_penelope->getY() && y <= m_penelope->getY()+SPRITE_HEIGHT-1) )
            return true;
        // check top right corner
        if( (x+SPRITE_WIDTH-1 >= m_penelope->getX() && x+SPRITE_WIDTH-1 <= m_penelope->getX()+SPRITE_WIDTH-1) && (y+SPRITE_HEIGHT-1 >= m_penelope->getY() && y+SPRITE_HEIGHT-1 <= m_penelope->getY()+SPRITE_HEIGHT-1) )
            return true;
    }
    
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    // iterate thru each and make sure does not overlap at ALL
    while(it != m_actors.end())
    {
        // if given Actor should block movement, check if within the square of the given actor - if it is then return true
            // i.e. check if each corner of given actor's box is within the other actor's box
        if((*it)->blocksMovement() && (*it) != a )
        {
            // check bottom left corner,
            if( (x >= (*it)->getX() && x <= (*it)->getX()+SPRITE_WIDTH-1) && (y >= (*it)->getY() && y <= (*it)->getY()+SPRITE_HEIGHT-1) )
                return true;
            // check top left corner
            if ( (x >= (*it)->getX() && x <= (*it)->getX()+SPRITE_WIDTH-1) && (y+SPRITE_HEIGHT-1 >= (*it)->getY() && y+SPRITE_HEIGHT-1 <= (*it)->getY()+SPRITE_HEIGHT-1) )
                return true;
            // check bottom right corner
            if( (x+SPRITE_WIDTH-1 >= (*it)->getX() && x+SPRITE_WIDTH-1 <= (*it)->getX()+SPRITE_WIDTH-1) && (y >= (*it)->getY() && y <= (*it)->getY()+SPRITE_HEIGHT-1) )
                return true;
            // check top right corner
            if( (x+SPRITE_WIDTH-1 >= (*it)->getX() && x+SPRITE_WIDTH-1 <= (*it)->getX()+SPRITE_WIDTH-1) && (y+SPRITE_HEIGHT-1 >= (*it)->getY() && y+SPRITE_HEIGHT-1 <= (*it)->getY()+SPRITE_HEIGHT-1) )
                return true;
        }
        // iterate thru all Actors
        it++;
    }
    // if all corners check out, return false (not blocked)
    return false;
}

bool StudentWorld::overlaps(int x1, int y1, int x2, int y2)
{
    int deltaX, deltaY;
    int dist = 10;      // given that if euclidian dist less than 10 overlaps
    
    deltaX = (x1 + .5*SPRITE_WIDTH) - (x2 + .5*SPRITE_WIDTH);
    deltaY = (y1 + .5*SPRITE_HEIGHT) - (y2 + .5*SPRITE_HEIGHT);
    
    return ( deltaX*deltaX + deltaY*deltaY <= dist*dist );
}


// determines if Exit overlaps w/ a Citizen given Exit's (x,y) -> delete that Citizen
bool StudentWorld::exitOverlapsWithCitizen(int x, int y)
{
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // check if current actor is a CITIZEN and if overlaps w/ Exit
            // Note: only Citizens are infectable
        if(overlaps(x,y,(*it)->getX(),(*it)->getY()) && (*it)->isInfectable())
        {
            (*it)->makeDead();
            return true;
        }
        it++;
    }
    return false;   // if get here means didn't overlap w/ any citizens
}



bool StudentWorld::worldContainsCitizens()       // used to determine if Penelope can exit
{
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // check if current actor is a Citizen, if it is return true (the world does contain citizens)
            // Note: only Citizens are infectable (except Penelope checked elsewhere
        if((*it)->isInfectable())
            return true;
        it++;
    }
    return false;   // if get here means didn't find any citizens
}

// given (x,y) of Exit, check if world still contains Citizens
bool StudentWorld::penelopeCanExit(int x, int y)
{
    return (!worldContainsCitizens() && overlaps(m_penelope->getX(), m_penelope->getY(), x,y) );
}


void StudentWorld::toggleTimeToAdvance()
{
    m_timeToAdvance = !m_timeToAdvance;
}



// called by Flame object every tick - will burnUp any flammable objects that overlap with it
void StudentWorld::burnOverlappingItems(int x, int y)
{
    // check Penelope because not included in Actor List
    if(overlaps(m_penelope->getX(),m_penelope->getY(),x,y))
        m_penelope->burnUp();
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // check if Actor is flammable
        if((*it)->isFlammable() && (*it)->isAlive() && overlaps(x,y,(*it)->getX(),(*it)->getY()))
            (*it)->burnUp(); 
        it++;
    }
}

// check to make sure wouldn't overlap w/ object that blocksFlame, then add. if can't add it then return false
bool StudentWorld::addFlame(int x, int y, Direction dir)
{
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    // check to make sure doesn't overlap w/ any actor which blocksFlame
    while(it != m_actors.end())
    {
        if((*it)->blocksFlame() && overlaps(x,y,(*it)->getX(), (*it)->getY()) )
            return false;
        
        it++;
    }
    
    // if didn't overlap w/ any actor which blocksFlame
    Actor* fire = new Flame(x,y,this);
    m_actors.push_back(fire);
    
    //  BELOW CUZ FLAMETHROWER FLAMES HAVE TO BE SAME DIRECTION PENELOPE FACING
    fire->setDirection(dir);
    
    return true;
}

// when Penelope presses spacebar tries to shoot flamethrower up to three spaces away in given direction. starts w/ (x,y) of Penelope
void StudentWorld::flamethrower(int x, int y)
{
    Direction dir = m_penelope->getDirection();
    
    switch (dir)
    {
        case GraphObject::left :
            // up to three spaces away
            for(int i = 1; i < 4; i++)
            {
                // if addFlame returns false could not add flame, STOP ADDING FLAMES
                if(!addFlame(x-i*SPRITE_WIDTH,y,GraphObject::left))
                {
                    break;
                }
            }
            break;
        case GraphObject::right :
            // up to three spaces away
            for(int i = 1; i < 4; i++)
            {
                // if addFlame returns false could not add flame
                if(!addFlame(x+i*SPRITE_WIDTH,y,GraphObject::right))
                    break;
            }
            break;
        case GraphObject::up :
            // up to three spaces away
            for(int i = 1; i < 4; i++)
            {
                // if addFlame returns false could not add flame
                if(!addFlame(x,y+i*SPRITE_HEIGHT,GraphObject::up))
                    break;
            }
            break;
        case GraphObject::down :
            // up to three spaces away
            for(int i = 1; i < 4; i++)
            {
                // if addFlame returns false could not add flame
                if(!addFlame(x,y-i*SPRITE_HEIGHT,GraphObject::down))
                    break;
            }
            break;
    }
    
}


// infect all Actors that overlap w/ vomit given (x,y) of vomit
void StudentWorld::infectOverlappingItems(int x, int y)
{
    // check Penelope because not included in Actor List
    if(m_penelope->isAlive() && overlaps(m_penelope->getX(),m_penelope->getY(),x,y))
        m_penelope->setInfectionStatus(true);
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // check if Actor is infectable (only citizens left) AND not already infected
        if((*it)->isInfectable() && (*it)->isAlive() && overlaps(x,y,(*it)->getX(),(*it)->getY()) && !((*it)->getInfectionStatus()))
        {
            playSound(SOUND_CITIZEN_INFECTED);
            (*it)->setInfectionStatus(true);
        }
        it++;
    }
}


// will see if Landmine overlaps w/ any Actors that trigger it (given (x,y) of landmine)
bool StudentWorld::checkOverlapExplodable(int x, int y, Actor* a)
{
    // check Penelope because not included in Actor List
    if(overlaps(m_penelope->getX(),m_penelope->getY(),x,y))
        return true;
    
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // check if Actor is flammable&blocksMovemnt (means AGENTS, FLAMES but NOT LANDMINES)
            // MAKE SURE DOESN'T OVERLAP W/ ITSELF
            // note: only Flames !flammable, !infectable, !blockMovement, !blockFlames
        if(( ( (*it)->isFlammable() && (*it)->blocksMovement() ) || !( (*it)->isFlammable() || (*it)->isInfectable() || (*it)->blocksMovement() || (*it)->blocksFlame())  ) && a != (*it) && overlaps(x,y,(*it)->getX(),(*it)->getY()))
        {
            return true;
        }

        it++;
    }
    // didn't find any overlapping explodable items
    return false;
}


// return the distance to the nearest zombie (used by Citizen in doSomething() )
int StudentWorld::distNearestZombie(int x, int y)
{
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    if(!containsZombies())
        return -1;
    
    int minDist = -2;       // so knows to change the first time
    int deltaX, deltaY, tempDist;
    
    while(it != m_actors.end())
    {
        // first check if is a Zombie
            // note: only Zombies are flammable, !infectable, blockMovement, !blockFlames
        if((*it)->isFlammable() && !((*it)->isInfectable()) && (*it)->blocksMovement() && !((*it)->blocksFlame()))
        {
            deltaX = (*it)->getX() - x;
            deltaY = (*it)->getY() - y;
            tempDist = sqrt(deltaX*deltaX + deltaY*deltaY);
            if(minDist == -2 || tempDist < minDist)
                minDist = tempDist;
        }
        it++;
    }
    
    return minDist;
}


bool StudentWorld::containsZombies()
{
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // check if current actor is a Zombie, if it is return true (the world does contain citizens)
            // note: only Zombies are flammable, !infectable, blockMovement, !blockFlames
        if((*it)->isFlammable() && !((*it)->isInfectable()) && (*it)->blocksMovement() && !((*it)->blocksFlame()))
            return true;
        it++;
    }
    return false;   // if get here means didn't find any citizens
}


// step 3 for Zombie doSomething() -> check if person in front of direction and if so compute Vomit stuff.
// input (x,y) of Zombie
// returns true if vommited, false otherwise
bool StudentWorld::checkPersonInFrontOfZombie(int x, int y, Direction dir)
{
    // used to calculate potential Vomit coordinates
    int vomitX = -1;
    int vomitY = -1;
    
    // compute vomit coordinates in direction Zombie is facing
    switch(dir)
    {
        case GraphObject::left :
            vomitX = x - SPRITE_WIDTH;
            vomitY = y;
            break;
        case GraphObject::right :
            vomitX = x + SPRITE_WIDTH;
            vomitY = y;
            break;
        case GraphObject::up :
            vomitX = x;
            vomitY = y + SPRITE_HEIGHT;
            break;
        case GraphObject::down :
            vomitX = x;
            vomitY = y - SPRITE_HEIGHT;
            break;
    }
    
    
    
    // now check if any objects overlap w/ Potential Vomit Coordinates
    
    // first check that there are valid vomit coordinates ( just in case smt went wrong)
    if(vomitX == -1 || vomitY == -1)    // did not get initialized
        return false;
    
    // check if infectable Actor intersects w/ potential vomit coordinates, if yes then calculate if Zombie should vomit 
    
    // random number to calculate if should vomit or not (1/3 chance will vomit)
    int rand = randInt(1,3);
    
    // check Penelope seperately (not included in list m_actors)
    if(overlaps(vomitX,vomitY, m_penelope->getX(), m_penelope->getY()) && rand == 2)
    {        
        // introduce new vomit to game at vomit coordinates
        addActor(new Vomit(vomitX,vomitY,this));
        // play sound effect
        playSound(SOUND_ZOMBIE_VOMIT);
        return true;
    }
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // recall random number each loop for truly random
        rand = randInt(1,3);
        
        // if overlaps and 1/3 chance to vomit and object is human (therefore infectable)
        if((*it)->isInfectable() && overlaps(vomitX, vomitY, (*it)->getX(), (*it)->getY()) && rand == 2)
        {
            // introduce new vomit to game at vomit coordinates
            addActor(new Vomit(vomitX,vomitY,this));
            // play sound effect
            playSound(SOUND_ZOMBIE_VOMIT);
            return true;
        }
        it++;
    }
    return false;
}


// does step 4 for SmartZombie doSomething(), called by SmartZombie's doSomething() given (x,y) of zombie 
void StudentWorld::smartZombieLogic(int x,int y, SmartZombie* z)
{
    // select person closest to smart zombie (smallest euclidian dist)
    int dist, deltaX, deltaY, tempDist;
    Actor* closestActor = m_penelope;       // start defualt Penelope closest relevant Actor
    
    // check penelope seperately since not included in m_actors
        // NOTE: there should always be a Penelope actor in the world
    deltaX = m_penelope->getX() - x;
    deltaY = m_penelope->getY() - y;
    dist = sqrt(deltaX*deltaX + deltaY*deltaY);
    
    // now check each human actor (only humans are infectable)
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // if Actor isInfectable and therefore a Human
        if((*it)->isInfectable())
        {
            // calculate euclidian dist
            deltaX = (*it)->getX() - x;
            deltaY = (*it)->getY() - y;
            tempDist = sqrt(deltaX*deltaX + deltaY*deltaY);
            // if distance is less than old distance, set this to distance
            if(tempDist < dist)
            {
                dist = tempDist;
                closestActor = *it;
            }
        }
        it++;
    }
    
    
    // if nearest distance is > 80 away, set direction to random direction
    if(dist > 80)
    {
        int rand = randInt(1,4);
        switch(rand)
        {
            case 1:
                z->setDirection(GraphObject::left);
                break;
            case 2:
                z->setDirection(GraphObject::right);
                break;
            case 3:
                z->setDirection(GraphObject::up);
                break;
            case 4:
                z->setDirection(GraphObject::down);
                break;
        }
    }
    
    // otherwise, direction is chosen to be the one that causes zombie to get closer to that person
    else
    {
        // to make code more compact
        int closeX = closestActor->getX();
        int closeY = closestActor->getY();
        
        // used to randomly choose one of two correct directions
        int rand = randInt(0,1);
        
        
        // case same row or column, choose only direction to make closer
        if(x == closeX)           // same col
        {
            if(y > closeY)        // zombie above Actor, should go down
                z->setDirection(GraphObject::down);
            else                                // zombie below Actor, should go up
                z->setDirection(GraphObject::up);
        }
        else if (y == closeY)     // same row
        {
            if(x > closeX)        // zombie to right Actor, should go left
                z->setDirection(GraphObject::left);
            else                                // zombie to left Actor, should go right
                z->setDirection(GraphObject::right);
        }
        
        
        // otherwise, choose randomly between two directions that get Zombie closer
            // Note: closeX != x and closeY != y at this point
        
        // closestActor TOP RIGHT -> should move up or right
        else if(closeY > y && closeX > x)
        {
            if(rand == 0)       // MOVE UP
            {
                z->setDirection(GraphObject::up);
            }
            else if(rand == 1)                // MOVE RIGHT
            {
                z->setDirection(GraphObject::right);
            }
        }
        // closestActor TOP LEFT -> should move up or left
        else if(closeY > y && closeX < x)
        {
            if(rand == 0)       // MOVE UP
            {
                z->setDirection(GraphObject::up);
            }
            else if(rand == 1)                // MOVE LEFT
            {
                z->setDirection(GraphObject::left);
            }
        }
        // closestActor BOTTOM LEFT -> should move down or left
        else if(closeY < y && closeX < x)
        {
            if(rand == 0)       // MOVE DOWN
            {
                z->setDirection(GraphObject::down);
            }
            else if(rand == 1)                // MOVE LEFT
            {
                z->setDirection(GraphObject::left);
            }
        }
        // closestActor BOTTOM RIGHT -> should move down or right
        else if(closeY < y && closeX > x)
        {
            if(rand == 0)       // MOVE DOWN
            {
                z->setDirection(GraphObject::down);
            }
            else if(rand == 1)                // MOVE RIGHT
            {
                z->setDirection(GraphObject::right);
            }
        }
    }
    
}


// called by Zombie upon dying -> needs to determine if vaccine threw will overlap w/ any Actor (if so return false)
// param (x,y) potential coordinate of VaccineGoodie
bool StudentWorld::overlapsWithAnything(int x, int y)
{
    // first check penelope since not included in m_actor
    if(overlaps(x,y,m_penelope->getX(),m_penelope->getY()))
        return true;
    
    // iterate thru all other Actors and check if overlaps
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // if Actor overlaps w/ coordinates return true (and NOT flame)
        if((*it)->isAlive() && overlaps(x,y,(*it)->getX(),(*it)->getY()))
            return true;
        it++;
    }
    
    // else didn't overlap w/ anything
    return false;
}


// called by Pit's doSomething() ... param (x,y) of Pit
void StudentWorld::pitExecuteLogic(int x, int y)
{
    // any peson, zombie (ANY FLAMMABLE OBJECT (since other ones can't move onto pit) which overlaps w/ Pit is destroyed - act as if burned by flame
    
    // first check penelope since not included in m_actors
    if(overlaps(m_penelope->getX(),m_penelope->getY(),x,y))
    {
        m_penelope->burnUp();
    }
    
    // go thru m_actors and call burnUp (since falling in Pit is same as being burned) for the FLAMMABLE objects
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // if Actor isFlammable and can therefore die by a pit AND overlaps w/ pit
        if((*it)->isFlammable() && overlaps((*it)->getX(),(*it)->getY(), x, y))
        {
            (*it)->burnUp();
        }
        it++;
    }
}
