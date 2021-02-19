#include "GameWorld.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <list>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath): GameWorld(assetPath) {
    m_player = nullptr;
    m_levelFinished = false;
    m_laneFound = false;
    m_actorFound = false;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    m_levelFinished = false;
    
    m_player = new GhostRacer(128, 32, this);
    double nObjects = VIEW_HEIGHT / SPRITE_HEIGHT;
    double mObjects = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    double leftEdge = ROAD_CENTER - (ROAD_WIDTH / 2);
    double rightEdge = ROAD_CENTER + (ROAD_WIDTH / 2);
    
    for (int i = 0; i < nObjects; i++) {
        m_actors.push_front(new BorderLine(1, leftEdge, i*SPRITE_HEIGHT, this));
        m_actors.push_front(new BorderLine(1, rightEdge, i*SPRITE_HEIGHT, this));
    }
    
    for (int i = 0; i < mObjects; i++) {
        m_actors.push_back(new BorderLine(2, leftEdge + (ROAD_WIDTH/3), i * (4*SPRITE_HEIGHT), this));
        m_actors.push_back(new BorderLine(2, rightEdge - (ROAD_WIDTH/3), i * (4*SPRITE_HEIGHT), this));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    Actor* actor = m_actors.back();
    double newBorderY = VIEW_HEIGHT-SPRITE_HEIGHT;
    double deltaY = newBorderY-(actor->getY());
    
    if (deltaY >= SPRITE_HEIGHT) {
        m_actors.push_front(new BorderLine(1, ROAD_CENTER - (ROAD_WIDTH/2), newBorderY, this));
        m_actors.push_front(new BorderLine(1, ROAD_CENTER + (ROAD_WIDTH/2), newBorderY, this));
    }

    if (deltaY >= 4*SPRITE_HEIGHT) {
        m_actors.push_back(new BorderLine(2, ROAD_CENTER - ((ROAD_WIDTH/2)-(ROAD_WIDTH/3)), newBorderY, this));
        m_actors.push_back(new BorderLine(2, ROAD_CENTER + ((ROAD_WIDTH/2)-(ROAD_WIDTH/3)), newBorderY, this));
    }
    
    double chanceOilSlick = max(150-getLevel()*10, 40);
    if (randInt(0, chanceOilSlick-1) == 0) {
        int xLocation = randInt(ROAD_CENTER - (ROAD_WIDTH / 2), ROAD_CENTER + (ROAD_WIDTH / 2));
        int size = randInt(2, 5);
        m_actors.push_front(new OilSlick(xLocation, VIEW_HEIGHT, size, this));
    }
    
    double chanceHolyWater = 100 + 10 * getLevel();
    if (randInt(0, chanceHolyWater-1) == 0) {
        int xLocation = randInt(ROAD_CENTER - (ROAD_WIDTH / 2), ROAD_CENTER + (ROAD_WIDTH / 2));
        m_actors.push_front(new HolyWaterGoodie(xLocation, VIEW_HEIGHT, this));
    }
    
    double chanceOfLostSoul = 100;
    if (randInt(0, chanceOfLostSoul-1) == 0) {
        int xLocation = randInt(ROAD_CENTER - (ROAD_WIDTH / 2), ROAD_CENTER + (ROAD_WIDTH / 2));
        m_actors.push_front(new SoulGoodie(xLocation, VIEW_HEIGHT, this));
    }
    
    double chanceHumanPed = max(200 - getLevel() * 10, 30);
    if (randInt(0, chanceHumanPed-1) == 0) {
        int xLocation = randInt(0, VIEW_WIDTH);
        m_actors.push_front(new HumanPedestrian(xLocation, VIEW_HEIGHT, this));
    }
    
    double chanceZombiePed = max(100 - getLevel() * 10, 30);
    if (randInt(0, chanceZombiePed-1) == 0) {
        int xLocation = randInt(0, VIEW_WIDTH);
        m_actors.push_front(new ZombiePedestrian(xLocation, VIEW_HEIGHT, this));
    }
    
    double chanceZombieCab = max(100 - getLevel() * 10, 20);
    if (randInt(0, chanceZombieCab-1) == 0) {
        int i = randInt(0, 2);
        if (i == 0) {
            if (!evalLeft()) {
                i = randInt(0, 1);
                if (i == 0) {
                    if (!evalMiddle())
                        evalRight();
                }
                else {
                    if (!evalRight())
                        evalMiddle();
                }
            }
        }
        else if (i == 1) {
            if (!evalMiddle()) {
                i = randInt(0, 1);
                if (i == 0) {
                    if (!evalLeft())
                        evalRight();
                }
                else {
                    if (!evalRight())
                        evalLeft();
                }
            }
        }
        else {
            if (!evalRight()) {
                i = randInt(0, 1);
                if (i == 0) {
                    if (!evalLeft())
                        evalMiddle();
                }
                else {
                    if (!evalMiddle())
                        evalLeft();
                }
            }
        }
     
    }
    
    m_player->doSomething();
    list<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        if ((*it)->isAlive())
            (*it)->doSomething();
        if (!m_player->isAlive()) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
    }
    
    
    it = m_actors.begin();
    while(it != m_actors.end()) {
        if (!(*it)->isAlive()) {
            delete *it;
            it = m_actors.erase(it);
            continue;
        }
        it++;
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it;
    it = m_actors.begin();
    while(it != m_actors.end()) {
        delete *it;
        it = m_actors.erase(it);
    }
    
    if (m_player != nullptr)
        delete m_player;
    m_player = nullptr;
    
}

// =========================================

GhostRacer* StudentWorld::getPlayer() const {
    return m_player;
}

int StudentWorld::getPlayerX() const {
    return m_player->getX();
}

int StudentWorld::getPlayerY() const {
    return m_player->getY();
}

double StudentWorld::getPlayerRadius() const {
    return m_player->getRadius();
}

int StudentWorld::getPlayerDirection() const {
    return m_player->getDirection();
}

bool StudentWorld::overlapsWith(int x1, int y1, double r1, int x2, int y2, double r2) const {
    double deltaX = abs(x1-x2);
    double deltaY = abs(y1-y2);
    double radiusSum = r1 + r2;
    
    if (deltaX < radiusSum * 0.25 && deltaY < radiusSum * 0.6)
        return true;
    return false;
}

bool StudentWorld::overlapsWithRacer(int x1, int y1, double radius) const {
    double deltaX = abs(x1-getPlayerX());
    double deltaY = abs(y1-getPlayerY());
    double radiusSum = radius + getPlayerRadius();
    
    if (deltaX < radiusSum * 0.25 && deltaY < radiusSum * 0.6)
        return true;
    return false;
    
}

bool StudentWorld::overlapsWithProjectile(int x1, int y1, double radius) const {
    list<Actor*>::const_iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        Actor* a = *it;
        if (a->isAlive() && (a->canBeDamagedByWater() || a->canDestroyOnHit()) && overlapsWith(x1, y1, radius, a->getX(), a->getY(), a->getRadius())) {
            if (a->canDestroyOnHit()) {
                a->setDead();
            }
            else {
                a->setHealth(a->getHealth()-1);
            }
            return true;
        }
        it++;
    }
    return false;
}

void StudentWorld::addWater(int x1, int y1) {
    int direction = getPlayerDirection();
    m_actors.push_front(new HolyWaterProjectile(x1, y1, direction, this));
}

bool StudentWorld::actorFront(int x1, int y1) {
    list<Actor*>::const_iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        Actor* a = *it;
        if (a->isAlive() && a->canBeDamaged()) {
            if (a->getX() == x1) {
                if (abs(a->getY()-y1) < 96)
                    return true;
            }
        }
        it++;
    }
    return false;
}

bool StudentWorld::actorBehind(int x1, int y1) {
    list<Actor*>::const_iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        Actor* a = *it;
        if (a->isAlive() && a->canBeDamagedByWater()) {
            if (a->getX() == x1) {
                if (abs(a->getY()-y1) < 96)
                    return true;
            }
        }
        it++;
    }
    return false;
}

void StudentWorld::setLaneFound() {
    m_laneFound = true;
}

bool StudentWorld::getLaneFound() {
    return m_laneFound;
}

void StudentWorld::resetLane() {
    m_laneFound = false;
}

void StudentWorld::setActorNotFound() {
    m_actorFound = true;
}

bool StudentWorld::getActorNotFound() {
    return m_actorFound;
}

void StudentWorld::resetActor() {
    m_actorFound = false;
}

bool StudentWorld::evalLeft() {
    list<Actor*>::const_iterator it = m_actors.begin();
    Actor* maxActor = nullptr;
    int closestPosition = 255;
    while (it != m_actors.end()) {
        Actor* a = *it;
        if (a->canBeDamaged()) {
            if (a->getX() >= left_border && a->getX() <= left_white_line) {
                if (a->getY() <= closestPosition) {
                    closestPosition = a->getY();
                    maxActor = a;
                }
            }
        }
        it++;
    }
    
    if (getPlayerX() >= left_border && getPlayerX() <= left_white_line) {
        if (getPlayerY() <= closestPosition) {
            closestPosition = getPlayerY();
            maxActor = getPlayer();
        }
    }
    
    if (maxActor == nullptr || maxActor->getY() > VIEW_HEIGHT/3) {
        double startY = SPRITE_HEIGHT/2;
        double startSpeed = getPlayer()->getvertSpeed() + randInt(2, 4);
        m_actors.push_front(new ZombieCab(ROAD_CENTER-ROAD_WIDTH/3, startY, startSpeed, this));
        return true;
    }
    
    it = m_actors.begin();
    maxActor = nullptr;
    closestPosition = 0;
    while (it != m_actors.end()) {
        Actor* a = *it;
        if (a->canBeDamaged()) {
           // cout << (a->getY());
            if (a->getX() >= left_border && a->getX() <= left_white_line) {
                if (a->getY() >= closestPosition) {
                    closestPosition = a->getY();
                    maxActor = a;
                }
            }
        }
        it++;
    }
    
    if (getPlayerX() >= left_border && getPlayerX() <= left_white_line) {
        if (getPlayerY() >= closestPosition) {
            closestPosition = getPlayerY();
            maxActor = getPlayer();
        }
    }
    
    if (maxActor == nullptr || maxActor->getY() < VIEW_HEIGHT*2/3) {
        double startY = VIEW_HEIGHT-SPRITE_HEIGHT/2;
        double startSpeed = getPlayer()->getvertSpeed() - randInt(2, 4);
        m_actors.push_front(new ZombieCab(ROAD_CENTER-ROAD_WIDTH/3, startY, startSpeed, this));
        return true;
    }
    
    return false;
}


bool StudentWorld::evalMiddle() {
    list<Actor*>::const_iterator it = m_actors.begin();
    Actor* maxActor = nullptr;
    int closestPosition = 255;
    while (it != m_actors.end()) {
        Actor* a = *it;
        if (a->canBeDamaged()) {
            if (a->getX() >= left_white_line && a->getX() <= right_white_line) {
                if (a->getY() <= closestPosition) {
                    closestPosition = a->getY();
                    maxActor = a;
                }
            }
        }
        it++;
    }
    
    if (getPlayerX() >= left_white_line && getPlayerX() <= right_white_line) {
        if (getPlayerY() <= closestPosition) {
            closestPosition = getPlayerY();
            maxActor = getPlayer();
        }
    }
    
    if (maxActor == nullptr || maxActor->getY() > VIEW_HEIGHT/3) {
        double startY = SPRITE_HEIGHT/2;
        double startSpeed = getPlayer()->getvertSpeed() + randInt(2, 4);
        m_actors.push_front(new ZombieCab(ROAD_CENTER, startY, startSpeed, this));
        return true;
    }
    
    it = m_actors.begin();
    maxActor = nullptr;
    closestPosition = 0;
    while (it != m_actors.end()) {
        Actor* a = *it;
        if (a->canBeDamaged()) {
            if (a->getX() >= left_white_line && a->getX() <= right_white_line) {
                if (a->getY() >= closestPosition) {
                    closestPosition = a->getY();
                    maxActor = a;
                }
            }
        }
        it++;
    }
    
    if (getPlayerX() >= left_white_line && getPlayerX() <= right_white_line) {
        if (getPlayerY() >= closestPosition) {
            closestPosition = getPlayerY();
            maxActor = getPlayer();
        }
    }
    
    if (maxActor == nullptr || maxActor->getY() > VIEW_HEIGHT/3) {
        double startY = VIEW_HEIGHT-SPRITE_HEIGHT/2;
        double startSpeed = getPlayer()->getvertSpeed() - randInt(2, 4);
        m_actors.push_front(new ZombieCab(ROAD_CENTER, startY, startSpeed, this));
        return true;
    }
    
    return false;
}


bool StudentWorld::evalRight() {
    list<Actor*>::const_iterator it = m_actors.begin();
    Actor* maxActor = nullptr;
    int closestPosition = 255;
    while (it != m_actors.end()) {
        Actor* a = *it;
        if (a->canBeDamaged()) {
            if (a->getX() >= right_white_line && a->getX() <= right_border) {
                if (a->getY() <= closestPosition) {
                    closestPosition = a->getY();
                    maxActor = a;
                }
            }
        }
        it++;
    }
    
    if (getPlayerX() >= right_white_line && getPlayerX() <= right_border) {
        if (getPlayerY() <= closestPosition) {
            closestPosition = getPlayerY();
            maxActor = getPlayer();
        }
    }
    
    if (maxActor == nullptr || maxActor->getY() > VIEW_HEIGHT/3) {
        double startY = SPRITE_HEIGHT/2;
        double startSpeed = getPlayer()->getvertSpeed() + randInt(2, 4);
        m_actors.push_front(new ZombieCab(ROAD_CENTER+ROAD_WIDTH/3, startY, startSpeed, this));
        return true;
    }
    
    it = m_actors.begin();
    maxActor = nullptr;
    closestPosition = 0;
    while (it != m_actors.end()) {
        Actor* a = *it;
        if (a->canBeDamaged()) {
            if (a->getX() >= right_white_line && a->getX() <= right_border) {
                if (a->getY() >= closestPosition) {
                    closestPosition = a->getY();
                    maxActor = a;
                }
            }
        }
        it++;
    }
    
    if (getPlayerX() >= right_white_line && getPlayerX() <= right_border) {
        if (getPlayerY() >= closestPosition) {
            closestPosition = getPlayerY();
            maxActor = getPlayer();
        }
    }
    
    if (maxActor == nullptr || maxActor->getY() > VIEW_HEIGHT/3) {
        double startY = VIEW_HEIGHT-SPRITE_HEIGHT/2;
        double startSpeed = getPlayer()->getvertSpeed() - randInt(2, 4);
        m_actors.push_front(new ZombieCab(ROAD_CENTER+ROAD_WIDTH/3, startY, startSpeed, this));
        return true;
    }
    
    return false;
}



