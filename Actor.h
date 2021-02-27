#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

const double max_shift_per_tick = 4.0;
const int max_travel_distance = 160;
const double left_border = ROAD_CENTER-ROAD_WIDTH/2;
const double left_white_line = ROAD_CENTER-ROAD_WIDTH/2+ROAD_WIDTH/3;
const double right_white_line = ROAD_CENTER+ROAD_WIDTH/2-ROAD_WIDTH/3;
const double right_border = ROAD_CENTER+ROAD_WIDTH/2;

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
    virtual ~Character() {};
    virtual bool hasHealth() const;
    virtual bool canBeDamaged() const;
    virtual bool canBeDamagedByWater() const;
    void setPlanDistance(int num);
    int getPlanDistance() const;
private:
    int m_planDistance;
};


class Obstacle : public Actor {
public:
    Obstacle(int imageID, int startX, int startY, int size, StudentWorld* sw);
    virtual ~Obstacle() {};
private:
};


class Goodie : public Actor {
public:
    Goodie(int imageID, int startX, int startY, int size, StudentWorld* sw);
    virtual ~Goodie() {};
    virtual void doSomething();
    virtual bool canDestroyOnHit() const;
    virtual void performGoodieEffect();
    virtual bool canLevel() const;
    virtual bool canHeal() const;
    virtual bool canRefill() const;
private:
    virtual void giveGoodie() = 0;
};


class HolyWaterProjectile : public Actor {
public:
    HolyWaterProjectile(double startX, double startY, int direction, StudentWorld *sw);
    virtual ~HolyWaterProjectile() {};
    virtual void doSomething();
private:
    int m_pixelsTraveled;
    void incrementPixels(int amount);
    int getPixelsTraveled() const;
};


class Vehicle : public Character {
public:
    Vehicle(int imageID, int startX, int startY, int health, StudentWorld *sw);
    virtual ~Vehicle() {};
private:
};

class GhostRacer : public Vehicle {
public:
    GhostRacer(int startX, int startY, StudentWorld* sw);
    virtual ~GhostRacer() {};
    virtual void doSomething();
    virtual bool canBeDamagedByWater() const;
    int getCharges() const;
    void addCharges(int num);
    void spin();
private:
    int m_waterCharges;
    void shootWater();
};

class ZombieCab : public Vehicle {
public:
    ZombieCab(int startX, int startY, int startSpeed, StudentWorld* sw);
    virtual ~ZombieCab() {};
    virtual void doSomething();
private:
    bool m_damagedRacer;
    int getLane() const;
    bool damagedGhostRacer() const;
    void racerDamaged();
};


class Pedestrian : public Character {
public:
    Pedestrian(int imageID, int startX, int startY, int size, StudentWorld *sw);
    virtual ~Pedestrian() {};
    void makeNewPlanDistance();
private:
    int m_planDistance;
};

class HumanPedestrian : public Pedestrian {
public:
    HumanPedestrian(int startX, int startY, StudentWorld *sw);
    virtual ~HumanPedestrian() {};
    virtual void doSomething();
    virtual bool redirectOnImpact() const;
};

class ZombiePedestrian : public Pedestrian {
public:
    ZombiePedestrian(int startX, int startY, StudentWorld* sw);
    virtual ~ZombiePedestrian() {};
    virtual void doSomething();
private:
    int m_ticksUntilGrunt;
    void setTicksUntilGrunt(int ticks);
    int getTicksUntilGrunt() const;
};


class BorderLine : public Obstacle {
public:
    BorderLine(int imageID, int startX, int startY, StudentWorld* sw);
    virtual ~BorderLine() {};
    virtual void doSomething();
};

class OilSlick : public Obstacle {
public:
    OilSlick(int startX, int startY, int size, StudentWorld *sw);
    virtual ~OilSlick() {};
    virtual void doSomething();
private:
};

 
class HealingGoodie : public Goodie {
public:
    HealingGoodie(int startX, int startY, StudentWorld* sw);
    virtual ~HealingGoodie() {};
    virtual bool canHeal() const;
private:
    virtual void giveGoodie();
};

class HolyWaterGoodie : public Goodie {
public:
    HolyWaterGoodie(int startX, int startY, StudentWorld* sw);
    virtual ~HolyWaterGoodie() {};
    virtual bool canRefill() const;
private:
    virtual void giveGoodie();
};

class SoulGoodie : public Goodie {
public:
    SoulGoodie(int startX, int startY, StudentWorld* sw);
    virtual ~SoulGoodie() {};
    virtual bool canDestroyOnHit() const;
    virtual bool canLevel() const;
private:
    virtual void giveGoodie();
};


#endif // ACTOR_H_
