#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


// Actor Class - Abstract Base Class
class Actor: public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, Direction dir, int depth, double size, bool alive, StudentWorld* world);
    virtual ~Actor();
    
    bool isAlive();
    virtual StudentWorld* getWorld();
    
    // pure virtual func since will never instantiate an Actor
    virtual void doSomething() = 0;
    
private:
    StudentWorld* m_myWorld;
    bool m_isAlive;
};


// Mobile class - for objects that move around the map either thru user input or algorithm
class Mobile: public Actor
{
public:
    Mobile(int imageID, double startX, double startY, Direction dir, int depth, double size, bool alive, StudentWorld* world);
    virtual ~Mobile();
    
    virtual bool isBlocked(int destX, int destY);
    virtual void doSomething() = 0;
};


// Wall Class
class Wall: public Actor
{
public:
    Wall(double startX, double startY, double size, StudentWorld* world);
    virtual ~Wall();
    virtual void doSomething();
};


// Penelope Class
class Penelope: public Mobile
{
public:
    Penelope(double startX, double startY, double size, StudentWorld* world);
    virtual ~Penelope();
    virtual void doSomething();
    
private:
    bool m_isInfected;
    int m_numMines;
    int m_numFlames;
    int m_numVac;
    int m_infectCount;
};



#endif // ACTOR_H_
