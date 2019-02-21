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
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    virtual ~StudentWorld();
    void loadLevel();
    
    bool overlaps(int x, int y, int dist);
    bool isBlocked(int x, int y);

private:
    Penelope* m_penelope;
    std::list<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
