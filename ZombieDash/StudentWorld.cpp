#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <sstream>
#include <list>
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
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
    if (result == Level::load_fail_file_not_found)
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
            return GWSTATUS_PLAYER_DIED;
        }
        
        // if all remaining Citizens and Penelope have used exit and time to advance to new level, return GWSTATUS_FINISHED_LEVEL
        
        // m_penelope->canExit() // check to make sure no more Citizens in world (Studentworld method to do this)
        // timeToAdvance has member variable in StudentWorld (canExit) which is toggled by Exit when
        
        // TODO: IMPLEMENT ADVANCING TO NEXT LEVEL CHECKER
        
//        if(timeToAdvance())
//            return GWSTATUS_FINISHED_LEVEL;
        
        it++;
    }
    


    // DELETE ANY ACTORS THAT HAVE DIED DURING THIS TICK AND REMOVE THEM FROM COLLECTION OF ACTORS
    
    it = m_actors.begin();      // reuse iterator from before
    // iterate thru all Actors and delete the dead ones
    while(it != m_actors.end())
    {
        if(!((*it)->isAlive()))     // if given Actor is dead
            it = m_actors.erase(it);    // not a Vector so this is ok, returns iterator pointing after erased
        else
            it++;
    }
    
    
    // UPDATE STATUS TEXT ON TOP OF SCREEN W/ LATEST INFO
    
    
    // TODO: update StatusText code
    

   
    
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    delete m_penelope;
    
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



// see if object should BLOCK movment (pg 10 spec)
bool StudentWorld::isBlocked(int x, int y)
{
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    // iterate thru each and make sure does not overlap at ALL
    while(it != m_actors.end())
    {
        // check if within square of given actor - if it is then return true
        // check if each corner of given actor's box is within the other actor's box
        
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
        
        it++;
        
    }
    // if all corners check out, return false (not blocked)
    return false;
}


// see if two objects overlap in map (input is bottom left corner of item)
bool StudentWorld::overlaps(int x, int y, Actor& a)
{
    // iterate thru all other actors and check if they overlap -> if they do then return true
    
    int deltaX, deltaY;
    int dist = 10;      // given that if euclidian dist less than 10 then ovelrap
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // modify values below to get centre of the object
        deltaX = ((*it)->getX() + .5*SPRITE_WIDTH) - (x + .5*SPRITE_WIDTH);
        deltaY = ((*it)->getY() + .5*SPRITE_HEIGHT) - (y + .5*SPRITE_HEIGHT);
        
        if ( deltaX*deltaX + deltaY*deltaY <= dist*dist )
        {
            return true;    // inersects
        }
        
        it++;
    }
    
    return false;       // there were no intersections
}



