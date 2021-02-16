#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor

Actor::Actor(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* sw): GraphObject(imageID, startX, startY, startDirection, size, depth) {
    m_studentWorld = sw;
    m_alive = true;
    m_vertSpeed = 0;
    m_horizSpeed = 0;
}

StudentWorld* Actor::getWorld() const {
    return m_studentWorld;
}

bool Actor::isAlive() const {
    return m_alive;
}

void Actor::setDead() {
    m_alive = false;
}

bool Actor::blocksMovement() const {
    return false;
}

void Actor::setvertSpeed(int speed) {
    m_vertSpeed = speed;
}

void Actor::sethorizSpeed(int speed) {
    m_horizSpeed = speed;
}

int Actor::getvertSpeed() const {
    return m_vertSpeed;
}

int Actor::gethorizSpeed() const {
    return m_horizSpeed;
}

// Ghost Racer ===========================================================================

GhostRacer::GhostRacer(int imageID, int startX, int startY, StudentWorld* sw):
    Actor(imageID, startX, startY, 90, 4.0, 0, sw) {
        
}

void GhostRacer::computeDestination(int& destX, int& destY, int direction) {
    double deltaX = cos(direction * 3.14159 / 180) * 4;
    destX += deltaX;
}

void GhostRacer::doSomething() {
    if (!isAlive())
        return;
    
    int ch;
    if (getWorld()->getKey(ch)) {
        bool directionKeyPressed = false;
        double currDirection = getDirection();
        int currSpeed = getvertSpeed();
        switch(ch) {
            case KEY_PRESS_LEFT:
                if (currDirection < 114)
                    setDirection(currDirection+8);
                
                directionKeyPressed = true;
                break;
            case KEY_PRESS_RIGHT:
                if (currDirection > 66)
                    setDirection(currDirection-8);
                directionKeyPressed = true;
                break;
            case KEY_PRESS_UP:
                if (currSpeed < 5)
                    setvertSpeed(currSpeed+1);
                directionKeyPressed = true;
                break;
            case KEY_PRESS_DOWN:
                if (currSpeed > -1)
                    setvertSpeed(currSpeed-1);
                directionKeyPressed = true;
                break;
        }
    }
    double currX = getX();
    double currY = getY();
    double currDirection = getDirection();
    double deltaX = cos(currDirection * M_PI / 180) * max_shift_per_tick;
    moveTo(currX+deltaX, currY);
}


// BorderLine ===================================================================

BorderLine::BorderLine(int imageID, int startX, int startY, StudentWorld *sw):
    Actor(imageID, startX, startY, 0, 2.0, 1, sw) {
        setvertSpeed(-4);
}

void BorderLine::doSomething() {
    GhostRacer* player = getWorld()->getPlayer();
    int vertSpeed = getvertSpeed() - player->getvertSpeed();
    int horizSpeed = gethorizSpeed();
    int destX = getX() + horizSpeed;
    int destY = getY() + vertSpeed;
    
    moveTo(destX, destY);
    
    if (destX < 0 || destY < 0 || destX > VIEW_WIDTH || destY > VIEW_HEIGHT) {
        setDead();
        return;
    }
}

