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
    // Given
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    // Accessors
    GhostRacer* getPlayer() const;
    int getPlayerX() const;
    int getPlayerY() const;
    double getPlayerRadius() const;
    int getPlayerDirection() const;
    // Mutators
    void decreaseSoulsToSave();
    void addWater(int x1, int y1);
    void addHealingGoodie(int x1, int y1);
    void addOilSlick(int x1, int y1);
    void heal();
    void addSprays();
    bool actorFront(int lane, Actor* current);
    bool actorBehind(int lane, Actor* current);
    bool overlapsWith(int x1, int y1, double r1, int x2, int y2, double r2) const;
    bool overlapsWithRacer(int x1, int y1, double radius) const;
    bool overlapsWithProjectile(int x1, int y1, double radius) const;
private:
    GhostRacer* m_player;
    std::list<Actor*> m_actors;
    int m_soulsToSave;
    int m_bonusPoints;
    
    int getSoulsToSave();
    int getBonusPoints();
    void setSoulsToSave(int num);
    void decreaseBonusPoints();
    bool evalLeft();
    bool evalMiddle();
    bool evalRight();
};

#endif // STUDENTWORLD_H_
