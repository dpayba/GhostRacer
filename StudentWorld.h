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
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    GhostRacer* getPlayer() const;
    int getPlayerX() const;
    int getPlayerY() const;
    double getPlayerRadius() const;
    int getPlayerDirection() const;
    bool overlapsWith(int x1, int y1, double r1, int x2, int y2, double r2) const;
    bool overlapsWithRacer(int x1, int y1, double radius) const;
    bool overlapsWithProjectile(int x1, int y1, double radius) const;
    void addWater(int x1, int y1);
    bool actorFront(int x1, int y1);
    bool actorBehind(int x1, int y1);
    void setLaneFound();
    bool getLaneFound();
    void resetLane();
//    Actor* checkBottomScreen(Actor*& actor, int leftBorder, int rightBorder);
//    Actor* checkTopScreen(Actor*& actor, int leftBorder, int rightBorder);
    void setActorNotFound();
    bool getActorNotFound();
    void resetActor();
    bool evalLeft();
    bool evalMiddle();
    bool evalRight();
private:
    GhostRacer* m_player;
    std::list<Actor*> m_actors;
    bool m_laneFound;
    bool m_actorFound;
    bool m_levelFinished;
};

#endif // STUDENTWORLD_H_
