#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

const double max_shift_per_tick = 4.0;

class Actor: public GraphObject {
public:
    Actor(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* sw);
    virtual ~Actor() {};
    StudentWorld* getWorld() const;
    virtual void doSomething() = 0;
    virtual bool isCollisionAvoidance();
    virtual bool canBeDamaged() const;
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
    GhostRacer(int startX, int startY, StudentWorld* sw);
    virtual ~GhostRacer() {};
    virtual void doSomething();
    void computeDestination(int& destX, int& destY, int direction);
    bool isOuterLine() const;
    int getHealth() const;
    void decreaseHealth(int amount);
    void spin();
private:
    int m_health;
};


class Obstacle : public Actor {
public:
    Obstacle(int imageID, int startX, int startY, int size, StudentWorld* sw);
    virtual ~Obstacle() {};
    void move();
private:
};


class BorderLine : public Obstacle {
public:
    BorderLine(int imageID, int startX, int startY, StudentWorld* sw);
    virtual ~BorderLine() {};
    virtual void doSomething();
    bool blocksMovement() const;
private:
    int m_racerSpeed;
    
};



class OilSlick : public Obstacle {
public:
    OilSlick(int startX, int startY, int size, StudentWorld *sw);
    virtual ~OilSlick() {};
    virtual bool canBeDamaged() const;
    virtual void doSomething();
private:
};

/**
class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY, StudentWorld* sw);
    virtual bool isCollisionAvoidance();
    virtual void doSomething();
private:
};
 */

#endif // ACTOR_H_
