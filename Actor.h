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
    void moveDown();
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
    void increaseHealth(int amount);
    void spin();
private:
    int m_health;
};


class Obstacle : public Actor {
public:
    Obstacle(int imageID, int startX, int startY, int size, StudentWorld* sw);
    virtual ~Obstacle() {};
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




class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY, int size, StudentWorld* sw);
    // hard code depth 2, same horiz and vert speed, direction 0, size changes
  //  virtual bool isCollisionAvoidance();
    virtual bool canHeal();
    virtual bool canRefill();
    virtual bool canLevel();
    virtual void doSomething();
private:
};
 
class HealingGoodie : public Goodie {
public:
    HealingGoodie(int startX, int startY, StudentWorld* sw);
    virtual bool canHeal();
private:
};

class HolyWaterGoodie : public Goodie {
public:
    HolyWaterGoodie(int startX, int startY, StudentWorld* sw);
    virtual bool canRefill();
private:
};

class SoulGoodie : public Goodie {
public:
    SoulGoodie(int startX, int startY, StudentWorld* sw);
    virtual bool canLevel();
private:
};




class Pedestrian : public Actor {
public:
    Pedestrian(int imageID, int startX, int startY, int size, StudentWorld *sw);
    void setPlanDistance(int num);
    int getPlanDistance() const;
    void makeNewPlanDistance();
private:
    int m_planDistance;
};

class HumanPedestrian : public Pedestrian {
public:
    HumanPedestrian(int startX, int startY, StudentWorld *sw);
    virtual void doSomething();
private:
};

class ZombiePedestrian : public Pedestrian {
public:
    ZombiePedestrian(int startX, int startY, StudentWorld* sw);
    virtual void doSomething();
    void setTicksUntilGrunt(int ticks);
    int getTicksUntilGrunt() const;
private:
    int m_ticksUntilGrunt;
};





#endif // ACTOR_H_
