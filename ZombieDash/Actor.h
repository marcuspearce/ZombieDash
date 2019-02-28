#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


// Actor Class - Abstract Base Class
// every class has to doSomething and be able to access the StudentWorld it is in
class Actor: public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, int depth, bool flammable, bool infectable, bool blockMove, bool blockFlame, StudentWorld* world);
    virtual ~Actor();
    virtual void doSomething() = 0;     // pure virtual func since will never instantiate an Actor

    
    virtual StudentWorld* getWorld() const;
    
    // check if item overlaps w/ another given the actor's location
//    bool overlaps(int x, int y, Actor& a);
    
    
    bool isAlive() const;
    void makeDead();

    bool isFlammable() const;
    bool isInfectable() const;
    bool blocksMovement() const;
    bool blocksFlame() const;
    
    // check if object can exit the level -> false for EVERYTHING except for Citizen and Penelope
    virtual bool canExit() const;
    
    virtual void burnUp();
    
private:
    StudentWorld* m_myWorld;
    bool m_isAlive;
    
    bool m_flammable;
    bool m_infectable;
    bool m_blocksMovement;
    bool m_blocksFlame;
};

// Wall Class
// walls do not do anything in doSomething
class Wall: public Actor
{
public:
    Wall(double startX, double startY, StudentWorld* world);
    virtual ~Wall();
    virtual void doSomething();
};




//      =============== NEW STUFF ===============



class ActivatingObject : public Actor
{
public:
    ActivatingObject(int imageID, double startX, double startY, int depth, bool flammable, bool blockFlame,  StudentWorld* world);
    virtual ~ActivatingObject();
    // ActivatingObject never going to be instantiated -> Abstract Base Class
    virtual void doSomething() = 0;
    
    // accessor and mutator for isActive
    bool isActive() const;
    void setActive(bool b);
private:
    bool m_active;
};

class Exit : public ActivatingObject
{
public:
    Exit(double startX, double startY, StudentWorld* world);
    virtual ~Exit();
    virtual void doSomething();
};

class Pit : public ActivatingObject
{
public:
    Pit(double startX, double startY, StudentWorld* world);
    virtual ~Pit();
    virtual void doSomething();
};

class Flame : public ActivatingObject
{
public:
    Flame(double startX, double startY, StudentWorld* world);
    virtual ~Flame();
    virtual void doSomething();
private:
    int m_lifeTicks;
};

class Vomit : public ActivatingObject
{
public:
    Vomit(double startX, double startY, StudentWorld* world);
    virtual ~Vomit();
    virtual void doSomething();
};

class Landmine : public ActivatingObject
{
public:
    Landmine(double startX, double startY, StudentWorld* world);
    virtual ~Landmine();
    virtual void doSomething();
    
    virtual void burnUp();
};

class Goodie : public ActivatingObject
{
public:
    Goodie(int ImageID, double startX, double startY, StudentWorld* world);
    virtual ~Goodie();
    virtual void doSomething();
private:
    // since will never instantiate a Goodie -> Abstract Base Class (each Goodie implements their own
    virtual void specificGoodieStuff() = 0;
};

class VaccineGoodie : public Goodie
{
public:
    VaccineGoodie(double startX, double startY, StudentWorld* world);
    virtual ~VaccineGoodie();
//    virtual void doSomething();
private:
    virtual void specificGoodieStuff();
};

class GasCanGoodie : public Goodie
{
public:
    GasCanGoodie(double startX, double startY, StudentWorld* world);
    virtual ~GasCanGoodie();
//    virtual void doSomething();
private:
    virtual void specificGoodieStuff();
};

class LandmineGoodie : public Goodie
{
public:
    LandmineGoodie(double startX, double startY, StudentWorld* world);
    virtual ~LandmineGoodie();
//    virtual void doSomething();
private:
    virtual void specificGoodieStuff();
};





// Agent Class - for objects that move around the map either thru user input or algorithm
class Agent: public Actor
{
public:
    Agent(int imageID, double startX, double startY, bool infectable, StudentWorld* world);
    virtual ~Agent();
    
    bool isBlocked(int destX, int destY);
//    bool overlaps(int x, int y);
    
    // since will never instantiate an Agent -> Abstract Base Class
    virtual void doSomething() = 0;
};

// Zombie Class
class Zombie : public Agent
{
public:
    Zombie(double startX, double startY, StudentWorld* world);
    virtual ~Zombie();
    
    virtual void doSomething();
    
    int getMovePlanDist() const;
    void setMovePlanDist(int i);
    
    virtual void burnUp();

private:
    // since will never instantiate a Zombie -> Abstract Base Class
    virtual void specificZombieStuff() = 0;
    int m_movePlanDist;        // movement plan distance
};

// Dumb Zombie Class
class DumbZombie : public Zombie
{
public:
    DumbZombie(double startX, double startY, StudentWorld* world);
    virtual ~DumbZombie();
private:
    virtual void specificZombieStuff();
};

// Smart Zombie Class
class SmartZombie : public Zombie
{
public:
    SmartZombie(double startX, double startY, StudentWorld* world);
    virtual ~SmartZombie();
private:
    virtual void specificZombieStuff();
};


// Human Class
class Human : public Agent
{
public:
    Human(int imageID, double startX, double startY, StudentWorld* world);
    virtual ~Human();
    //since will never instantiate a Human -> Abstract Base Class
    virtual void doSomething() = 0;
    
    bool getInfectionStatus() const;
    void setInfectionStatus(bool b);
    
    int getInfectionCount() const;
    void incInfectionCount();       // increment infectionCount by 1
    
private:
    bool m_infectionStatus;
    int m_infectionCount;
};



// Citizen Class
class Citizen : public Human
{
public:
    Citizen(double startX, double startY, StudentWorld* world);
    virtual ~Citizen();
    virtual void doSomething();
    
    virtual bool canExit() const;
    
    virtual void burnUp();
};

// Penelope Class
class Penelope: public Human
{
public:
    Penelope(double startX, double startY, StudentWorld* world);
    virtual ~Penelope();
    virtual void doSomething();
    
    int getNumVaccines() const;
    int getNumFlames() const;
    int getNumMines() const;
    
    void incNumVaccines(int n);
    void incNumFlames(int n);
    void incNumMines(int n);
    
    virtual void burnUp();

    
//    virtual bool canExit() const;
    
private:
    int m_numVac;
    int m_numFlames;
    int m_numMines;
};






#endif // ACTOR_H_
