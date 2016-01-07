//
// Created by ranjam on 15-12-28.
//

#include "Tank.h"

float Tank::p1_velocity = 2.0f;
float Tank::p2_velocity = 2.0f;
float Tank::enemy1_velocity = 1.0f;
float Tank::enemy2_velocity = 3.0f;
float Tank::enemy3_velocity = 0.5f;

Tank *Tank::create(TANKTYPE tanktype_value, DIRECTION direction_value) {
    Tank *tank = new (std::nothrow)Tank();
    if (tank && tank->init(tanktype_value, direction_value)){
        tank->autorelease();
        return tank;
    }
    CC_SAFE_DELETE(tank);
    return nullptr;
}

bool Tank::init(Tank::TANKTYPE tanktype_value, DIRECTION direction_value, int life_value) {
    setTanktype(tanktype_value);
    setDirection(direction_value);
    this->initLife();
    return initWithFile(_tankname);
}

DIRECTION Tank::getDirection() const {
    return _direction;
}

void Tank::setDirection(DIRECTION direction_value) {
    setFace(direction_value);
    this->_direction = direction_value;
    switch (_direction){
        case UP:
            _tankname[0] = 'U';
            break;
        case DOWN:
            _tankname[0] = 'D';
            break;
        case LEFT:
            _tankname[0] = 'L';
            break;
        case RIGHT:
            _tankname[0] = 'R';
            break;
        case STOP:
            break;
    }
    initWithFile(_tankname);
}

Tank::TANKTYPE Tank::getTanktype() const {
    return _type;
}

void Tank::setTanktype(Tank::TANKTYPE tanktype_value) {
    this->_type = tanktype_value;
    switch (_type){
        case P1:
            strcpy(_tankname, "Up1tank.png");
            _group = PLAYER;
            break;
        case P2:
            strcpy(_tankname, "Up2tank.png");
            _group = PLAYER;
            break;
        case ENEMY1:
            strcpy(_tankname, "Uenemy1.png");
            _group = ENEMY;
            break;
        case ENEMY2:
            strcpy(_tankname, "Uenemy2.png");
            _group = ENEMY;
            break;
        case ENEMY3:
            strcpy(_tankname, "Uenemy3.png");
            _group = ENEMY;
            break;
    }
    initWithFile(_tankname);
}

void Tank::moveUp() {
    switch (_type){
        case P1:
            this->setPosition(Vec2(this->getPositionX(),this->getPositionY() + Tank::p1_velocity));
            break;
        case P2:
            this->setPosition(Vec2(this->getPositionX(),this->getPositionY() + Tank::p2_velocity));
            break;
        case ENEMY1:
            this->setPosition(Vec2(this->getPositionX(),this->getPositionY() + Tank::enemy1_velocity));
            break;
        case ENEMY2:
            this->setPosition(Vec2(this->getPositionX(),this->getPositionY() + Tank::enemy2_velocity));
            break;
        case ENEMY3:
            this->setPosition(Vec2(this->getPositionX(),this->getPositionY() + Tank::enemy3_velocity));
            break;
    }
}

void Tank::moveDown() {
    switch (_type){
        case P1:
            this->setPosition(Vec2(this->getPositionX(),this->getPositionY() - Tank::p1_velocity));
            break;
        case P2:
            this->setPosition(Vec2(this->getPositionX(),this->getPositionY() - Tank::p2_velocity));
            break;
        case ENEMY1:
            this->setPosition(Vec2(this->getPositionX(),this->getPositionY() - Tank::enemy1_velocity));
            break;
        case ENEMY2:
            this->setPosition(Vec2(this->getPositionX(),this->getPositionY() - Tank::enemy2_velocity));
            break;
        case ENEMY3:
            this->setPosition(Vec2(this->getPositionX(),this->getPositionY() - Tank::enemy3_velocity));
            break;
    }
}

void Tank::moveLeft() {
    switch (_type){
        case P1:
            this->setPosition(Vec2(this->getPositionX() - Tank::p1_velocity,this->getPositionY()));
            break;
        case P2:
            this->setPosition(Vec2(this->getPositionX() - Tank::p2_velocity,this->getPositionY()));
            break;
        case ENEMY1:
            this->setPosition(Vec2(this->getPositionX() - Tank::enemy1_velocity,this->getPositionY()));
            break;
        case ENEMY2:
            this->setPosition(Vec2(this->getPositionX() - Tank::enemy2_velocity,this->getPositionY()));
            break;
        case ENEMY3:
            this->setPosition(Vec2(this->getPositionX() - Tank::enemy3_velocity,this->getPositionY()));
            break;
    }
}

void Tank::moveRight() {
    switch (_type){
        case P1:
            this->setPosition(Vec2(this->getPositionX() + Tank::p1_velocity,this->getPositionY()));
            break;
        case P2:
            this->setPosition(Vec2(this->getPositionX() + Tank::p2_velocity,this->getPositionY()));
            break;
        case ENEMY1:
            this->setPosition(Vec2(this->getPositionX() + Tank::enemy1_velocity,this->getPositionY()));
            break;
        case ENEMY2:
            this->setPosition(Vec2(this->getPositionX() + Tank::enemy2_velocity,this->getPositionY()));
            break;
        case ENEMY3:
            this->setPosition(Vec2(this->getPositionX() + Tank::enemy3_velocity,this->getPositionY()));
            break;
    }
}

char *Tank::get_name() {
    return _tankname;
}

void Tank::born() {
    Animate *born = Animate::create(AnimationCache::getInstance()->getAnimation("born"));
    this->runAction(born);
    _lifetime = 0;
    _isborn = true;
    scheduleUpdate();
}

float Tank::getLifeTime() {
    return _lifetime;
}

bool Tank::isBorn() {
    return _isborn;
}

void Tank::update(float dt) {
    _lifetime += dt;
    if (_lifetime >= 1.65f){
        _isborn = false;
    }
}

int Tank::getLife() {
    return _life;
}

void Tank::setLife(int life_value) {
    _life = life_value;
}

void Tank::initLife() {
    switch (this->_type){
        case P1:
            _life = 1;
            break;
        case P2:
            _life = 1;
            break;
        case ENEMY1:
            _life = 1;
            break;
        case ENEMY2:
            _life = 1;
            break;
        case ENEMY3:
            _life = 3;
            break;
    }
}

void Tank::loseLife() {
    _life--;
}

DIRECTION Tank::getFace() {
    return _faceDirection;
}

void Tank::setFace(DIRECTION direction) {
    if (direction != STOP){
        _faceDirection = direction;
    }
}

GROUP Tank::getGroup() {
    return _group;
}
