#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

const double max_shift_per_tick = 4.0;
const int max_travel_distance = 160;

class Actor: public GraphObject {
public:
    Actor(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* sw);
    virtual ~Actor() {};
    virtual void doSomething() = 0;
    // Accessors
    StudentWorld* getWorld() const;
    virtual bool canBeDamaged() const;
    virtual bool canBeDamagedByWater() const;
    bool isAlive() const;
    bool blocksMovement() const;
    double getvertSpeed() const;
    double gethorizSpeed() const;
    virtual bool hasHealth() const;
    virtual bool canDestroyOnHit() const;
    int getHealth() const;
    // Mutators
    void setDead();
    void setvertSpeed(double speed);
    void sethorizSpeed(double speed);
    void moveDown();
    void setHealth(int amount);
    virtual void performGoodieEffect();
    virtual bool redirectOnImpact() const;
    void reverseDirection();
private:
    StudentWorld* m_studentWorld;
    bool m_alive;
    double m_vertSpeed;
    double m_horizSpeed;
    int m_health;
};



class Character : public Actor {
public:
    Character(int imageID, int startX, int startY, int startDirection, double size, int health, StudentWorld *sw);
    virtual bool hasHealth() const;
    virtual bool canBeDamaged() const;
    virtual bool canBeDamagedByWater() const;
private:
};




class Vehicle : public Character {
public:
    Vehicle(int imageID, int startX, int startY, int health, StudentWorld *sw);
private:
};




class GhostRacer : public Vehicle {
public:
    GhostRacer(int startX, int startY, StudentWorld* sw);
    virtual ~GhostRacer() {};
    virtual void doSomething();
    virtual bool canBeDamagedByWater() const;
    void computeDestination(int& destX, int& destY, int direction);
    bool isOuterLine() const;
    void spin();
    int getCharges() const;
    void addCharges(int num);
    void shootWater();
private:
    int m_waterCharges;
};

class ZombieCab : public Vehicle {
public:
    ZombieCab(int startX, int startY, int startSpeed, StudentWorld* sw);
    virtual void doSomething();
    bool damagedGhostRacer() const;
    void racerDamaged();
    void setPlanDistance(int num);
    int getPlanDistance() const;
    int getLane();
private:
    bool m_damagedRacer;
    int m_planDistance;
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
    virtual void doSomething();
private:
};




class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY, int size, StudentWorld* sw);
    virtual void doSomething();
    virtual bool canDestroyOnHit() const;
    virtual void performGoodieEffect();
    virtual bool canLevel();
    virtual bool canHeal();
    virtual bool canRefill();
private:
    virtual void giveGoodie() = 0;
};
 
class HealingGoodie : public Goodie {
public:
    HealingGoodie(int startX, int startY, StudentWorld* sw);
    virtual bool canHeal();
private:
    virtual void giveGoodie();
};

class HolyWaterGoodie : public Goodie {
public:
    HolyWaterGoodie(int startX, int startY, StudentWorld* sw);
    virtual bool canRefill();
private:
    virtual void giveGoodie();
};

class SoulGoodie : public Goodie {
public:
    SoulGoodie(int startX, int startY, StudentWorld* sw);
    virtual bool canDestroyOnHit() const;
    virtual bool canLevel();
private:
    virtual void giveGoodie();
};


class Pedestrian : public Character {
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
    virtual bool redirectOnImpact() const;
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




class HolyWaterProjectile : public Actor {
public:
    HolyWaterProjectile(double startX, double startY, int direction, StudentWorld *sw);
    virtual void doSomething();
    void incrementPixels();
    int getPixelsTraveled() const;
private:
    int m_pixelsTraveled;
};





#endif // ACTOR_H_
