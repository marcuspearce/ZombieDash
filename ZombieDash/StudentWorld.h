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
    
    
    
    
    bool isBlocked(int x, int y);
//    bool overlaps(int x, int y);
    bool overlaps(int x1, int y1, int x2, int y2);
    
    bool overlapsWithPenelope(int x, int y);
    
    // below for Exit logic
    bool worldContainsCitizens();       // used to determine if Penelope can exit
    void citizenExit(int x, int y);                 // find any citizens overlapping w/ Exit (given (x,y) of exit)
    bool penelopeCanExit(int x, int y);                // check if Penelope overlapping w/ Exit (given (x,y) of exit)
    void toggleTimeToAdvance();
    
    
    void burnOverlappingItems(int x, int y);
    bool addFlame(int x, int y);
    void flamethrower(int x, int y);

private:
    Penelope* m_penelope;
    std::list<Actor*> m_actors;
    
    bool m_timeToAdvance;
};

#endif // STUDENTWORLD_H_
