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

bool Actor::isCollisionAvoidance() {
    return true;
}

bool Actor::isAlive() const {
    return m_alive;
}

void Actor::setDead() {
    m_alive = false;
}

bool Actor::canBeDamaged() const {
    return true;
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

void Actor::moveDown() {
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

// Ghost Racer ========================================================================

GhostRacer::GhostRacer(int startX, int startY, StudentWorld* sw):
    Actor(0, startX, startY, 90, 4.0, 0, sw) {
        m_health = 100;
}

int GhostRacer::getHealth() const{
    return m_health;
}

void GhostRacer::decreaseHealth(int amount) {
    m_health -= amount;
}

void GhostRacer::increaseHealth(int amount) {
    m_health += amount;
}

void GhostRacer::spin() {
    int adjustDegrees = randInt(5, 20);
    int direction = randInt(0, 1);
    // 0 counterclockwise, 1 clockwise
    switch(direction) {
        case 0: // counterclockwise, increase angle
            if (getDirection() + adjustDegrees > 120)
                setDirection(120);
            else
                setDirection(getDirection() + adjustDegrees);
            break;
        case 1:
            if (getDirection() - adjustDegrees < 60)
                setDirection(60);
            else
                setDirection(getDirection() - adjustDegrees);
    }
}

void GhostRacer::doSomething() {
    if (!isAlive())
        return;
    
    if (getX() <= ROAD_CENTER - (ROAD_WIDTH / 2)) {
        if (getDirection() > 90) {
            decreaseHealth(10);
            setDirection(82);
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
        }
    }
    
    if (getX() >= ROAD_CENTER + (ROAD_WIDTH / 2)) {
        if (getDirection() < 90) {
            decreaseHealth(10);
            setDirection(98);
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
        }
    }
    
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

// Obstacle ===================================================================

Obstacle::Obstacle(int imageID, int startX, int startY, int size, StudentWorld* sw) :
    Actor(imageID, startX, startY, 0, size, 2, sw) {
        setvertSpeed(-4);
}


// BorderLine ===================================================================

BorderLine::BorderLine(int imageID, int startX, int startY, StudentWorld *sw):
    Obstacle(imageID, startX, startY, 2, sw) {
     
}

void BorderLine::doSomething() {
    moveDown();
}

// Oil Slick ===========================================================================

OilSlick::OilSlick(int startX, int startY, int size, StudentWorld* sw) :
    Obstacle(3, startX, startY, size, sw) {}

bool OilSlick::canBeDamaged() const {
    return false;
}

void OilSlick::doSomething() {
    moveDown();
    if (getWorld()->overlapsWithRacer(getX(), getY(), getRadius())) {
        getWorld()->playSound(SOUND_OIL_SLICK);
        getWorld()->getPlayer()->spin();
    }
}

// Goodie ============================================================

Goodie::Goodie(int imageID, int startX, int startY, int size, StudentWorld* sw) :
    Actor(imageID, startX, startY, 0, size, 2, sw) {
        setvertSpeed(-4);
}

void Goodie::doSomething() {
    moveDown();
    if (getWorld()->overlapsWithRacer(getX(), getY(), getRadius())) {
        if (canHeal()) {
            getWorld()->getPlayer()->decreaseHealth(10);
            setDead();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            // increase player score by 250
        }
        if (canRefill()) {
            // incrase holy water by 10
            setDead();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            // increase player score by 50
        }
        if (canLevel()) {
            // increase number of souls saved, may result in level end. studentworld must communicate to framework
            setDead();
            getWorld()->playSound(SOUND_GOT_SOUL);
            // increase score by 100
        }
    }
    if (canLevel())
        setDirection(getDirection() - 10);
    
}

bool Goodie::canHeal() {
    return false;
}

bool Goodie::canRefill() {
    return false;
}

bool Goodie::canLevel() {
    return false;
}

// Healing Goodie ============================================================

HealingGoodie::HealingGoodie(int startX, int startY, StudentWorld* sw) :
    Goodie(8, startX, startY, 1, sw) {}

bool HealingGoodie::canHeal() {
    return true;
}

// Holy Water Goodie ============================================================
 
HolyWaterGoodie::HolyWaterGoodie(int startX, int startY, StudentWorld* sw) :
    Goodie(10, startX, startY, 2, sw) {}

bool HolyWaterGoodie::canRefill() {
    return true;
}

// Soul Goodie ============================================================

SoulGoodie::SoulGoodie(int startX, int startY, StudentWorld* sw) :
    Goodie(9, startX, startY, 4, sw) {}

bool SoulGoodie::canLevel() {
    return true;
}
