#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor

Actor::Actor(int imageID, int startX, int startY, int startDirection, double size, int depth, StudentWorld* sw): GraphObject(imageID, startX, startY, startDirection, size, depth) {
    m_studentWorld = sw;
    m_alive = true;
    m_vertSpeed = 0;
    m_horizSpeed = 0;
    m_health = 0;
}

StudentWorld* Actor::getWorld() const {
    return m_studentWorld;
}

bool Actor::canBeDamaged() const {
    return false;
}

bool Actor::canBeDamagedByWater() const {
    return false;
}

bool Actor::isAlive() const {
    return m_alive;
}

double Actor::getvertSpeed() const {
    return m_vertSpeed;
}

double Actor::gethorizSpeed() const {
    return m_horizSpeed;
}

bool Actor::hasHealth() const {
    return false;
}
bool Actor::canDestroyOnHit() const {
    return false;
}

int Actor::getHealth() const{
    return m_health;
}


void Actor::setDead() {
    m_alive = false;
}

void Actor::setvertSpeed(double speed) {
    m_vertSpeed = speed;
}

void Actor::sethorizSpeed(double speed) {
    m_horizSpeed = speed;
}

void Actor::moveDown() {
    GhostRacer* player = getWorld()->getPlayer();
    double vertSpeed = getvertSpeed() - player->getvertSpeed();
    double horizSpeed = gethorizSpeed();
    double destX = getX() + horizSpeed;
    double destY = getY() + vertSpeed;
    
    moveTo(destX, destY);
    
    if (destX < 0 || destY < 0 || destX > VIEW_WIDTH || destY > VIEW_HEIGHT) {
        setDead();
        return;
    }
}

void Actor::setHealth(int amount) {
    if (amount > 100) {
        m_health = 100;
        return;
    }
    m_health = amount;
}
void Actor::performGoodieEffect() {}

bool Actor::redirectOnImpact() const {
    return false;
}

void Actor::reverseDirection() {
    sethorizSpeed(gethorizSpeed()*-1);
    if (getDirection() == 0)
        setDirection(180);
    else
        setDirection(0);
    getWorld()->playSound(SOUND_PED_HURT);
}

// Character

Character::Character(int imageID, int startX, int startY, int startDirection, double size, int health, StudentWorld *sw) :
Actor(imageID, startX, startY, startDirection, size, 0, sw) {
    setHealth(health);
    m_planDistance = 0;
}

bool Character::hasHealth() const {
    return true;
}

bool Character::canBeDamaged() const {
    return true;
}

bool Character::canBeDamagedByWater() const {
    return true;
}

void Character::setPlanDistance(int num) {
    m_planDistance = num;
}

int Character::getPlanDistance() const {
    return m_planDistance;
}

// Obstacle

Obstacle::Obstacle(int imageID, int startX, int startY, int size, StudentWorld* sw) :
    Actor(imageID, startX, startY, 0, size, 2, sw) {
        setvertSpeed(-4);
}

// Goodie

Goodie::Goodie(int imageID, int startX, int startY, int size, StudentWorld* sw) :
    Actor(imageID, startX, startY, 0, size, 2, sw) {
        setvertSpeed(-4);
}

void Goodie::doSomething() {
    moveDown();
    if (getWorld()->overlapsWithRacer(getX(), getY(), getRadius())) {
        giveGoodie();
    }
   
    if (canLevel())
        setDirection(getDirection() - 10);
    
}

bool Goodie::canDestroyOnHit() const {
    return true;
}

void Goodie::performGoodieEffect() {
    if (canHeal()) {
        getWorld()->heal();
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(250);
    }
    if (canRefill()) {
        
        getWorld()->addSprays();
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(50);
    }
}

bool Goodie::canHeal() const {
    return false;
}

bool Goodie::canRefill() const {
    return false;
}

bool Goodie::canLevel() const {
    return false;
}

// HolyWaterProjectile

HolyWaterProjectile::HolyWaterProjectile(double startX, double startY, int direction, StudentWorld *sw) :
    Actor(IID_HOLY_WATER_PROJECTILE, startX, startY, direction, 1, 1, sw) {
        m_pixelsTraveled = 0;
}

void HolyWaterProjectile::doSomething() {
    if (!isAlive())
        return;
    
    if (getWorld()->overlapsWithProjectile(getX(), getY(), getRadius())) {
        setDead();
        return;
    }
    
    moveForward(SPRITE_HEIGHT);
    incrementPixels(SPRITE_HEIGHT);
    
    if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
        setDead();
        return;
    }
    
    if (getPixelsTraveled() == max_travel_distance)
        setDead();
    
}

void HolyWaterProjectile::incrementPixels(int amount) {
    m_pixelsTraveled+=amount;
}

int HolyWaterProjectile::getPixelsTraveled() const {
    return m_pixelsTraveled;
}

// Vehicle

Vehicle::Vehicle(int imageID, int startX, int startY, int health, StudentWorld *sw) :
Character(imageID, startX, startY, 90, 4.0, health, sw) {}

// Ghost Racer

GhostRacer::GhostRacer(int startX, int startY, StudentWorld* sw):
    Vehicle(IID_GHOST_RACER, startX, startY, 100, sw) {
        m_waterCharges = 10;
}

void GhostRacer::doSomething() {
    if (getHealth() <= 0) {
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
        return;
    }
    
    if (getX() <= ROAD_CENTER - (ROAD_WIDTH / 2)) {
        if (getDirection() > 90) {
            setHealth(getHealth()-10);
            setDirection(82);
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
        }
    }
    
    if (getX() >= ROAD_CENTER + (ROAD_WIDTH / 2)) {
        if (getDirection() < 90) {
            setHealth(getHealth()-10);
            setDirection(98);
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
        }
    }
    
    int ch;
    if (getWorld()->getKey(ch)) {
        double currDirection = getDirection();
        double currSpeed = getvertSpeed();
        switch(ch) {
            case KEY_PRESS_SPACE:
                if (m_waterCharges > 0)
                    shootWater();
                break;
            case KEY_PRESS_LEFT:
                if (currDirection < 114)
                    setDirection(currDirection+8);
                break;
            case KEY_PRESS_RIGHT:
                if (currDirection > 66)
                    setDirection(currDirection-8);
                break;
            case KEY_PRESS_UP:
                if (currSpeed < 5)
                    setvertSpeed(currSpeed+1);
                break;
            case KEY_PRESS_DOWN:
                if (currSpeed > -1)
                    setvertSpeed(currSpeed-1);
                break;
        }
    }
    double currX = getX();
    double currY = getY();
    double currDirection = getDirection();
    double deltaX = cos(currDirection * M_PI / 180) * max_shift_per_tick;
    moveTo(currX+deltaX, currY);
}

bool GhostRacer::canBeDamagedByWater() const {
    return false;
}

int GhostRacer::getCharges() const {
    return m_waterCharges;
}

void GhostRacer::addCharges(int num) {
    m_waterCharges += num;
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

void GhostRacer::shootWater() {
    if (m_waterCharges <= 0)
        return;
    m_waterCharges--;
    getWorld()->playSound(SOUND_PLAYER_SPRAY);
    double xLoc = getX();
    double yLoc = getY();
    getPositionInThisDirection(getDirection(), 1, xLoc, yLoc);
    getWorld()->addWater(xLoc, yLoc);
}

// ZombieCab

ZombieCab::ZombieCab(int startX, int startY, int startSpeed, StudentWorld* sw) :
    Vehicle(IID_ZOMBIE_CAB, startX, startY, 3, sw) {
        m_damagedRacer = false;
        setvertSpeed(startSpeed);
}

void ZombieCab::doSomething() {
    GhostRacer* player = getWorld()->getPlayer();
    if (getHealth() <= 0) {
        setDead();
        getWorld()->playSound(SOUND_VEHICLE_DIE);
        if (randInt(0, 4) == 0)
            getWorld()->addOilSlick(getX(), getY());
        getWorld()->increaseScore(200);
        return;
    }
    
    if (!damagedGhostRacer() && getWorld()->overlapsWithRacer(getX(), getY(), getRadius())) {
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
        player->setHealth(player->getHealth()-20);
        
        if (getX() <= player->getX()) {
            sethorizSpeed(-5);
            int randDirection = randInt(0, 19);
            setDirection(120+randDirection);
        }
        
        if (getX() > player->getX()) {
            sethorizSpeed(5);
            int randDirection = randInt(0, 19);
            setDirection(60-randDirection);
        }
        
        racerDamaged();
    }
    
    moveDown();
    
    if (getvertSpeed() > player->getvertSpeed() && getWorld()->actorFront(getLane(), this)) {
        setvertSpeed(getvertSpeed()-0.5);
        return;
    }
    
    if (getvertSpeed() <= player->getvertSpeed() && getWorld()->actorBehind(getLane(), this)) {
        setvertSpeed(getvertSpeed()+0.5);
        return;
    }
    
    setPlanDistance(getPlanDistance()-1);
    
    if (getPlanDistance() > 0)
        return;
    
    setPlanDistance(randInt(4, 32));
    setvertSpeed(getvertSpeed() + randInt(-2, 2));
}

int ZombieCab::getLane() const {
    if (getX() >= left_border && getX() < left_white_line)
        return 0;
    else if (getX() >= left_white_line && getX() < right_white_line)
        return 1;
    else
        return 2;
}

bool ZombieCab::damagedGhostRacer() const {
    return m_damagedRacer;
}

void ZombieCab::racerDamaged() {
    m_damagedRacer = true;
}

// Pedestrian

Pedestrian::Pedestrian(int imageID, int startX, int startY, int size, StudentWorld *sw) : Character(imageID, startX, startY, 0, size, 2, sw) {
    setvertSpeed(-4);
}
void Pedestrian::makeNewPlanDistance() {
    int randSpeed = 0;
    while (randSpeed == 0) {
        randSpeed = randInt(-3, 3);
    }

    sethorizSpeed(randSpeed);
    setPlanDistance(randInt(4, 32));
    if (gethorizSpeed() < 0)
        setDirection(180);
    
    if (gethorizSpeed() > 0)
        setDirection(0);
}

// Human Pedestrian

HumanPedestrian::HumanPedestrian(int startX, int startY, StudentWorld *sw) :
    Pedestrian(IID_HUMAN_PED, startX, startY, 2.0, sw) {}

void HumanPedestrian::doSomething() {
    if (!isAlive())
        return;
    
    if (getWorld()->overlapsWithRacer(getX(), getY(), getRadius())) {
        getWorld()->getPlayer()->setDead();
    }
    
    moveDown();
    
    setPlanDistance(getPlanDistance()-1);
    if (getPlanDistance() > 0)
        return;
    
    makeNewPlanDistance();
}

bool HumanPedestrian::redirectOnImpact() const {
    return true;
}

// ZombiePedestrian

ZombiePedestrian::ZombiePedestrian(int startX, int startY, StudentWorld *sw) :
    Pedestrian(IID_ZOMBIE_PED, startX, startY, 3, sw) {
        m_ticksUntilGrunt = 0;
}

void ZombiePedestrian::doSomething() {
    if (getHealth() == 1)
        getWorld()->playSound(SOUND_PED_HURT);
    
    if (getHealth() <= 0) {
        setDead();
        getWorld()->playSound(SOUND_PED_DIE);
        if (!getWorld()->overlapsWithRacer(getX(), getY(), getRadius())) {
            if (randInt(0, 4) == 0)
                getWorld()->addHealingGoodie(getX(), getY());
        }
        getWorld()->increaseScore(150);
        return;
    }
    
        
    if (getWorld()->overlapsWithRacer(getX(), getY(), getRadius())) {
        getWorld()->getPlayer()->setHealth(getWorld()->getPlayer()->getHealth()-5);
        setHealth(getHealth()-2);
        return;
    }
    
    if (abs(getX() - getWorld()->getPlayerX()) <= 30 && getY() > getWorld()->getPlayerY()) {
        setDirection(270);
        if (getX() < getWorld()->getPlayerX())
            sethorizSpeed(1);
        else if (getX() > getWorld()->getPlayerX())
            sethorizSpeed(-1);
        else
            sethorizSpeed(0);
        
        setTicksUntilGrunt(getTicksUntilGrunt()-1);
        if (getTicksUntilGrunt() <= 0) {
            getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
            setTicksUntilGrunt(20);
        }
    }
    
    moveDown();
    
    if (getPlanDistance() > 0) {
        setPlanDistance(getPlanDistance()-1);
        return;
    }
    
    makeNewPlanDistance();
}

void ZombiePedestrian::setTicksUntilGrunt(int ticks) {
    m_ticksUntilGrunt = ticks;
}

int ZombiePedestrian::getTicksUntilGrunt() const {
    return m_ticksUntilGrunt;
}

// BorderLine

BorderLine::BorderLine(int imageID, int startX, int startY, StudentWorld *sw):
    Obstacle(imageID, startX, startY, 2, sw) {}

void BorderLine::doSomething() {
    moveDown();
}

// Oil Slick

OilSlick::OilSlick(int startX, int startY, int size, StudentWorld* sw) :
    Obstacle(IID_OIL_SLICK, startX, startY, size, sw) {}

void OilSlick::doSomething() {
    moveDown();
    if (getWorld()->overlapsWithRacer(getX(), getY(), getRadius())) {
        getWorld()->playSound(SOUND_OIL_SLICK);
        getWorld()->getPlayer()->spin();
    }
}

// Healing Goodie

HealingGoodie::HealingGoodie(int startX, int startY, StudentWorld* sw) :
    Goodie(IID_HEAL_GOODIE, startX, startY, 1, sw) {}

bool HealingGoodie::canHeal() const {
    return true;
}

void HealingGoodie::giveGoodie() {
    getWorld()->heal();
    setDead();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(250);
}

// Holy Water Goodie
 
HolyWaterGoodie::HolyWaterGoodie(int startX, int startY, StudentWorld* sw) :
    Goodie(IID_HOLY_WATER_GOODIE, startX, startY, 2, sw) {
        setDirection(90);
    }

bool HolyWaterGoodie::canRefill() const {
    return true;
}

void HolyWaterGoodie::giveGoodie() {
    getWorld()->addSprays();
    setDead();
    getWorld()->playSound(SOUND_GOT_GOODIE);
    getWorld()->increaseScore(50);
}

// Soul Goodie

SoulGoodie::SoulGoodie(int startX, int startY, StudentWorld* sw) :
    Goodie(IID_SOUL_GOODIE, startX, startY, 4, sw) {}

bool SoulGoodie::canDestroyOnHit() const {
    return false;
}

bool SoulGoodie::canLevel() const {
    return true;
}


void SoulGoodie::giveGoodie() {
    getWorld()->decreaseSoulsToSave();
    setDead();
    getWorld()->playSound(SOUND_GOT_SOUL);
    getWorld()->increaseScore(100);
}


