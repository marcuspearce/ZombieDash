#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    
    Penelope* getPenelope();
    
    void addActor(Actor* a);
    
    
    
    
    bool isBlocked(int x, int y, Actor* a);
//    bool overlaps(int x, int y);
    bool overlaps(int x1, int y1, int x2, int y2);
    
//    bool overlapsWithPenelope(int x, int y);
    
    // below for Exit logic
    bool exitOverlapsWithCitizen(int x, int y);
    bool penelopeCanExit(int x, int y);                // check if Penelope overlapping w/ Exit (given (x,y) of exit)
    void toggleTimeToAdvance();
    
    // below for Flame logic
    void burnOverlappingItems(int x, int y);
    bool addFlame(int x, int y, Direction dir);
    void flamethrower(int x, int y);
    
    // below for Vomit logic
    void infectOverlappingItems(int x, int y);
    
    // below for Landmine logic
    bool checkOverlapExplodable(int x, int y, Actor* a);
    
    // below for Citizen logic
    int distNearestZombie(int x, int y);
    bool containsZombies();
    
    // below for Zombie logic
    bool checkPersonInFrontOfZombie(int x, int y, Direction dir);
    
    // below for SmartZombie logic
    void smartZombieLogic(int x, int y, SmartZombie* z);
    
    // below for dumbZombieLogic
    bool overlapsWithAnything(int x, int y);
    
    // below for Pit logic
    void pitExecuteLogic(int x, int y);

private:
    Penelope* m_penelope;
    std::list<Actor*> m_actors;
    
    // helper function for penelopeCanExit() func
    bool worldContainsCitizens();       // used to determine if Penelope can exit
    
    bool m_timeToAdvance;

};

#endif // STUDENTWORLD_H_
