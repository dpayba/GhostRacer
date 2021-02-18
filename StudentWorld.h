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
    GhostRacer* getPlayer() const;
    int getPlayerX() const;
    int getPlayerY() const;
    double getPlayerRadius() const;
    bool overlapsWith(int x1, int y1, double r1, int x2, int y2, double r2) const;
    bool overlapsWithRacer(int x1, int y1, double radius) const;
    bool overlapsWithProjectile(int x1, int y1, double radius) const;
    void addWater(int x1, int y1, double direction);
private:
    GhostRacer* m_player;
    std::list<Actor*> m_actors;
    bool m_levelFinished;
};

#endif // STUDENTWORLD_H_
