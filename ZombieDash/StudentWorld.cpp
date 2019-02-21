#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
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

int StudentWorld::init()
{
    loadLevel();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    
    // make Penelope doSomething if she's alive
    if(m_penelope->isAlive())
        m_penelope->doSomething();
    // TODO: check if died then return GWSTATUS_PLAYER_DIED
    // TODO: check if completed level then return GWSTATUS_FINISHED_LEVEL
    
    
    // iterate thru list of Actors and make them doSomething if they're alive
    list<Actor*>::iterator it;
    it = m_actors.begin();
    while(it != m_actors.end())
    {
        if((*it)->isAlive())
            (*it)->doSomething();
        it++;
    }
    
    // TODO: remove and delete dead game objects
    
    // TODO: update display text
    
    
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


void StudentWorld::loadLevel()
{
    Level lev(assetPath());
    string levelFile = "level01.txt";
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found)
        cerr << "Cannot find level01.txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "Your level was improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        
        for(int i = 0; i < 16; i++)
        {
            for(int j = 0; j < 16; j++)
            {
                Level::MazeEntry me = lev.getContentsOf(i,j);
                
                switch(me)
                {
                    case Level::player :
                        m_penelope = new Penelope(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 1.0, this);
                        break;
                    case Level::wall :
                        m_actors.push_back(new Wall(i*SPRITE_WIDTH, j*SPRITE_HEIGHT, 1.0, this));
                        break;
                    default:
                        break;      // for now do nothing
                }
            }
        }
    }
}


// see if two objects overlap in map
bool StudentWorld::overlaps(int x, int y, int dist)
{
    // iterate thru all other actors and check if they intersect -> if they do then return true
    
    int deltaX, deltaY;
    
    // initialize iterator
    list<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        // modify values below to get centre of the object
        deltaX = ((*it)->getX() + .5*SPRITE_WIDTH) - (x + .5*SPRITE_WIDTH);
        deltaY = ((*it)->getY() + .5*SPRITE_HEIGHT) - (y + .5*SPRITE_HEIGHT);
        
        if ( deltaX*deltaX + deltaY*deltaY <= dist*dist )
            return true;    // intersects
        
        it++;
    }
    
    return false;       // there were no intersections
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
