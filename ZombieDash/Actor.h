#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


// Actor Class - all other classes derived from this -> ABSTRACT BASE CLASS
class Actor: public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, int depth, bool flammable, bool infectable, bool blockMove, bool blockFlame, StudentWorld* world);
    virtual ~Actor();
    // Each Actor in the StudentWorld must doSomething() every tick of the game. I chose to define a pure virtual function of doSomething() in my base Actor class because all subclasses of Actor need to doSomething(), and each Actor doSomething() in a different way. 
    virtual void doSomething() = 0;     

    StudentWorld* getWorld() const;
    
    bool isAlive() const;
    void makeDead();

    bool isFlammable() const;
    bool isInfectable() const;
    bool blocksMovement() const;
    bool blocksFlame() const;
    
    // burnUp() will only be called on FLAMMABLE objects - some classes may expand on this behaviour
    virtual void burnUp();
    
    void setInfectionStatus(bool b);
    bool getInfectionStatus() const;

private:
    StudentWorld* m_myWorld;
    bool m_isAlive;
    
    bool m_flammable;
    bool m_infectable;
    bool m_blocksMovement;
    bool m_blocksFlame;
    bool m_infectionStatus;
};


// Wall Class
class Wall: public Actor
{
public:
    Wall(double startX, double startY, StudentWorld* world);
    virtual ~Wall();
    virtual void doSomething();
};


// ActivatingObject Class -> ABSTRACT BASE CLASS
class ActivatingObject : public Actor
{
public:
    ActivatingObject(int imageID, double startX, double startY, int depth, bool flammable, bool blockFlame,  StudentWorld* world);
    virtual ~ActivatingObject();
    virtual void doSomething() = 0;
    
    // accessor and mutator for isActive
    bool isActive() const;
    void setActive(bool b);
private:
    bool m_active;
};


// Exit Class
class Exit : public ActivatingObject
{
public:
    Exit(double startX, double startY, StudentWorld* world);
    virtual ~Exit();
    virtual void doSomething();
};


// Pit Class
class Pit : public ActivatingObject
{
public:
    Pit(double startX, double startY, StudentWorld* world);
    virtual ~Pit();
    virtual void doSomething();
};


// Flame Class
class Flame : public ActivatingObject
{
public:
    Flame(double startX, double startY, StudentWorld* world);
    virtual ~Flame();
    virtual void doSomething();
private:
    int m_lifeTicks;
};


// Vomit Class
class Vomit : public ActivatingObject
{
public:
    Vomit(double startX, double startY, StudentWorld* world);
    virtual ~Vomit();
    virtual void doSomething();
private:
    int m_lifeTicks;
};


// Landmine Class
class Landmine : public ActivatingObject
{
public:
    Landmine(double startX, double startY, StudentWorld* world);
    virtual ~Landmine();
    virtual void doSomething();
    
    virtual void burnUp();
private:
    void explode();
    int m_safetyTicks;
};


// Goodie Class -> ABSTRACT BASE CLASS
class Goodie : public ActivatingObject
{
public:
    Goodie(int ImageID, double startX, double startY, StudentWorld* world);
    virtual ~Goodie();
    virtual void doSomething();
private:
    virtual void specificGoodieStuff() = 0;
};



// VaccineGoodie Class
class VaccineGoodie : public Goodie
{
public:
    VaccineGoodie(double startX, double startY, StudentWorld* world);
    virtual ~VaccineGoodie();
//    virtual void doSomething();
private:
    virtual void specificGoodieStuff();
};


// GasCanGoodie Class
class GasCanGoodie : public Goodie
{
public:
    GasCanGoodie(double startX, double startY, StudentWorld* world);
    virtual ~GasCanGoodie();
//    virtual void doSomething();
private:
    virtual void specificGoodieStuff();
};


// LandmineGoodie Class
class LandmineGoodie : public Goodie
{
public:
    LandmineGoodie(double startX, double startY, StudentWorld* world);
    virtual ~LandmineGoodie();
//    virtual void doSomething();
private:
    virtual void specificGoodieStuff();
};


// Agent Class -> ABSTRACT BASE CLASS
class Agent: public Actor
{
public:
    Agent(int imageID, double startX, double startY, bool infectable, StudentWorld* world);
    virtual ~Agent();
    virtual void doSomething() = 0;
    
    bool isBlocked(int destX, int destY, Actor* a);
    
    void incParalysisTick();
    bool isParalyzed();
private:
    int m_paralysisTick;
};


// Zombie Class - ABSTRACT BASE CLASS
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
    virtual int specificZombieDie() = 0;
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
    virtual int specificZombieDie();
};


// Smart Zombie Class
class SmartZombie : public Zombie
{
public:
    SmartZombie(double startX, double startY, StudentWorld* world);
    virtual ~SmartZombie();
private:
    virtual void specificZombieStuff();
    virtual int specificZombieDie();
};


// Human Class - ABSTRACT BASE CLASS
class Human : public Agent
{
public:
    Human(int imageID, double startX, double startY, StudentWorld* world);
    virtual ~Human();
    virtual void doSomething() = 0;
        
    int getInfectionCount() const;
    void setInfectionCount(int i);       
    
private:
    int m_infectionCount;
};


// Citizen Class
class Citizen : public Human
{
public:
    Citizen(double startX, double startY, StudentWorld* world);
    virtual ~Citizen();
    virtual void doSomething();
    
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
private:
    int m_numVac;
    int m_numFlames;
    int m_numMines;
};






#endif // ACTOR_H_
