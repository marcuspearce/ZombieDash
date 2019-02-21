#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


// ACTOR member funciton implementations


Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, double size, bool alive, StudentWorld* world)
: GraphObject(imageID, startX, startY, dir, depth, size), m_myWorld(world), m_isAlive(alive)
{}

Actor::~Actor()
{}

bool Actor::isAlive()
{
    return m_isAlive;
}

// need way to access StudentWorld
StudentWorld* Actor::getWorld()
{
    return m_myWorld;
}


// MOBILE member function implementations
Mobile::Mobile(int imageID, double startX, double startY, Direction dir, int depth, double size, bool alive, StudentWorld* world)
: Actor(imageID, startX, startY, dir, depth, size, alive, world)
{}
Mobile::~Mobile()
{}

// using box around object see if is blocked from moving in given direction
bool Mobile::isBlocked(int destX, int destY)
{
    return getWorld()->isBlocked(destX, destY);
}


// WALL member function implementations


Wall::Wall(double startX, double startY, double size, StudentWorld* world)
: Actor(IID_WALL, startX, startY, right, 0, size, true, world)
{}

Wall::~Wall()
{}

// do nothing, I'm just a wall!
void Wall::doSomething()
{}


// PENELOPE member function implementations


Penelope::Penelope(double startX, double startY, double size, StudentWorld* world)
: Mobile(IID_PLAYER, startX, startY, right, 0, size, true, world), m_isInfected(false), m_numMines(0), m_numFlames(0), m_numVac(0), m_infectCount(0)
{}

Penelope::~Penelope()
{}

void Penelope::doSomething()
{
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
