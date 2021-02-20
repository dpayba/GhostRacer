#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

const double left_border = ROAD_CENTER-ROAD_WIDTH/2;
const double left_white_line = ROAD_CENTER-ROAD_WIDTH/2+ROAD_WIDTH/3;
const double right_white_line = ROAD_CENTER+ROAD_WIDTH/2-ROAD_WIDTH/3;
const double right_border = ROAD_CENTER+ROAD_WIDTH/2;

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
    int getSoulsToSave();
    int getBonusPoints();
    int getPlayerX() const;
    int getPlayerY() const;
    double getPlayerRadius() const;
    int getPlayerDirection() const;
    bool getLaneFound();
    bool getActorNotFound();
    // Mutators
    void setSoulsToSave(int num);
    void decreaseSoulsToSave();
    void decreaseBonusPoints();
    void addWater(int x1, int y1);
    void setLaneFound();
    void resetLane();
    void setActorNotFound();
    void resetActor();
    void addHealingGoodie(int x1, int y1);
    void addOilSlick(int x1, int y1);
    void heal();
    void addSprays();
    bool actorFront(int lane, int yPos);
    bool overlapsWith(int x1, int y1, double r1, int x2, int y2, double r2) const;
    bool overlapsWithRacer(int x1, int y1, double radius) const;
    bool overlapsWithProjectile(int x1, int y1, double radius) const;
    bool evalLeft();
    bool evalMiddle();
    bool evalRight();
private:
    GhostRacer* m_player;
    std::list<Actor*> m_actors;
    bool m_laneFound;
    bool m_actorFound;
    int m_soulsToSave;
    int m_bonusPoints;
};

#endif // STUDENTWORLD_H_
