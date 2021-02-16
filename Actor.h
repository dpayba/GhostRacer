#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

const double max_shift_per_tick = 4.0;

class Actor: public GraphObject {
public:
    Actor(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* sw);
    StudentWorld* getWorld() const;
    virtual void doSomething() = 0;
    bool isAlive() const;
    void setDead();
    bool blocksMovement() const;
    void setvertSpeed(int speed);
    void sethorizSpeed(int speed);
    int getvertSpeed() const;
    int gethorizSpeed() const;
private:
    StudentWorld* m_studentWorld;
    bool m_alive;
    int m_vertSpeed;
    int m_horizSpeed;
    
};



class GhostRacer : public Actor {
public:
    GhostRacer(int imageID, int startX, int startY, StudentWorld* sw);
    virtual void doSomething();
    void computeDestination(int& destX, int& destY, int direction);
    bool isOuterLine() const;
private:
};



class BorderLine : public Actor {
public:
    BorderLine(int imageID, int startX, int startY, StudentWorld* sw);
    virtual void doSomething();
    bool blocksMovement() const;
private:
    int m_racerSpeed;
    
};


#endif // ACTOR_H_
