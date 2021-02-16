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
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    m_levelFinished = false;
    
    m_player = new GhostRacer(0, 128, 32, this);
    
    
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
