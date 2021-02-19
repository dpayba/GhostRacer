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

bool Actor::isAlive() const {
    return m_alive;
}

void Actor::setDead() {
    m_alive = false;
}

bool Actor::canBeDamaged() const {
    return false;
}

bool Actor::canBeDamagedByWater() const {
    return false;
}

bool Actor::blocksMovement() const {
    return false;
}

void Actor::setvertSpeed(double speed) {
    m_vertSpeed = speed;
}

void Actor::sethorizSpeed(double speed) {
    m_horizSpeed = speed;
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

void Actor::setHealth(int amount) {
    m_health = amount;
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

// Character ========================================================================

Character::Character(int imageID, int startX, int startY, int startDirection, double size, int health, StudentWorld *sw) :
Actor(imageID, startX, startY, startDirection, size, 0, sw) {
    setHealth(health);
}

bool Character::canBeDamagedByWater() const {
    return true;
}

bool Character::hasHealth() const {
    return true;
}

bool Character::canBeDamaged() const {
    return true;
}

// Vehicle ========================================================================

Vehicle::Vehicle(int imageID, int startX, int startY, int health, StudentWorld *sw) :
Character(imageID, startX, startY, 90, 4.0, health, sw) {
}

// Ghost Racer ========================================================================

GhostRacer::GhostRacer(int startX, int startY, StudentWorld* sw):
    Vehicle(IID_GHOST_RACER, startX, startY, 100, sw) {
        m_waterCharges = 10;
}

int GhostRacer::getCharges() const {
    return m_waterCharges;
}

void GhostRacer::addCharges(int num) {
    m_waterCharges += num;
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

bool GhostRacer::canBeDamagedByWater() const {
    return false;
}

void GhostRacer::doSomething() {
    if (getHealth() <= 0) {
        setDead();
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
    Obstacle(IID_OIL_SLICK, startX, startY, size, sw) {}

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
            getWorld()->getPlayer()->setHealth(getWorld()->getPlayer()->getHealth()-10);
            setDead();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            // increase player score by 250
        }
        if (canRefill()) {
            getWorld()->getPlayer()->addCharges(10);
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

bool Goodie::canDestroyOnHit() const {
    return true;
}


// Healing Goodie ============================================================

HealingGoodie::HealingGoodie(int startX, int startY, StudentWorld* sw) :
    Goodie(IID_HEAL_GOODIE, startX, startY, 1, sw) {}

bool HealingGoodie::canHeal() {
    return true;
}

// Holy Water Goodie ============================================================
 
HolyWaterGoodie::HolyWaterGoodie(int startX, int startY, StudentWorld* sw) :
    Goodie(IID_HOLY_WATER_GOODIE, startX, startY, 2, sw) {}

bool HolyWaterGoodie::canRefill() {
    return true;
}

// Soul Goodie ============================================================

SoulGoodie::SoulGoodie(int startX, int startY, StudentWorld* sw) :
    Goodie(IID_SOUL_GOODIE, startX, startY, 4, sw) {}

bool SoulGoodie::canLevel() {
    return true;
}

bool SoulGoodie::canDestroyOnHit() const {
    return false;
}

// Pedestrian =====================================================================

Pedestrian::Pedestrian(int imageID, int startX, int startY, int size, StudentWorld *sw) : Character(imageID, startX, startY, 0, size, 2, sw) {
    
    m_planDistance = 0;
    setvertSpeed(-4);
}

void Pedestrian::setPlanDistance(int num) {
    m_planDistance = num;
}

int Pedestrian::getPlanDistance() const {
    return m_planDistance;
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

// Human Pedestrian =================================================================

HumanPedestrian::HumanPedestrian(int startX, int startY, StudentWorld *sw) :
    Pedestrian(IID_HUMAN_PED, startX, startY, 2.0, sw) {}

void HumanPedestrian::doSomething() {
    if (!isAlive())
        return;
    
    if (getWorld()->overlapsWithRacer(getX(), getY(), getRadius())) {
        // decrement life
        getWorld()->getPlayer()->setDead();
    }
    
    moveDown();
    
    setPlanDistance(getPlanDistance()-1);
    if (getPlanDistance() > 0)
        return;
    
    makeNewPlanDistance();
    
}

// ZombiePedestrian =========================================================

ZombiePedestrian::ZombiePedestrian(int startX, int startY, StudentWorld *sw) :
    Pedestrian(IID_ZOMBIE_PED, startX, startY, 3, sw) {
        m_ticksUntilGrunt = 0;
}

void ZombiePedestrian::setTicksUntilGrunt(int ticks) {
    m_ticksUntilGrunt = ticks;
}

int ZombiePedestrian::getTicksUntilGrunt() const {
    return m_ticksUntilGrunt;
}

void ZombiePedestrian::doSomething() {
    if (getHealth() <= 0) {
        setDead();
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


// HolyWaterProjectile =========================================================

HolyWaterProjectile::HolyWaterProjectile(double startX, double startY, int direction, StudentWorld *sw) :
    Actor(IID_HOLY_WATER_PROJECTILE, startX, startY, direction, 1, 1, sw) {
        m_pixelsTraveled = 0;
}

void HolyWaterProjectile::incrementPixels() {
    m_pixelsTraveled++;
}

int HolyWaterProjectile::getPixelsTraveled() const {
    return m_pixelsTraveled;
}

void HolyWaterProjectile::doSomething() {
    if (!isAlive())
        return;
    
    if (getWorld()->overlapsWithProjectile(getX(), getY(), getRadius())) {
        setDead();
        return;
    }
    
    moveForward(SPRITE_HEIGHT);
    incrementPixels();
    
    if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT) {
        setDead();
        return;
    }
    
    if (getPixelsTraveled() == max_travel_distance)
        setDead();
    
}


// ZombieCab =========================================================

ZombieCab::ZombieCab(int startX, int startY, StudentWorld* sw) :
    Vehicle(IID_ZOMBIE_CAB, startX, startY, 3, sw) {
        m_damagedRacer = false;
        m_planDistance = 0;
}

bool ZombieCab::damagedGhostRacer() const {
    return m_damagedRacer;
}

void ZombieCab::racerDamaged() {
    m_damagedRacer = true;
}

void ZombieCab::setPlanDistance(int num) {
    m_planDistance = num;
}

int ZombieCab::getPlanDistance() const {
    return m_planDistance;
}

void ZombieCab::doSomething() {
    GhostRacer* player = getWorld()->getPlayer();
    if (!isAlive())
        return;
    
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
    if (getvertSpeed() > player->getvertSpeed() && getWorld()->actorFront(getX(), getY())) {
        setvertSpeed(getvertSpeed()-0.5);
    }
    
    if (getvertSpeed() <= player->getvertSpeed() && getWorld()->actorFront(getX(), getY())) {
        setvertSpeed(getvertSpeed()+0.5);
    }
    
    setPlanDistance(getPlanDistance()-1);
    
    if (getPlanDistance() > 0)
        return;
    
    setPlanDistance(randInt(4, 32));
    setvertSpeed(getvertSpeed() + double(randInt(-2, 2)));
}
