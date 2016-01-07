//
// Created by ranjam on 16-1-2.
//

#include "BulletLayer.h"

bool BulletLayer::init() {
    if (!Layer::init()){
        return false;
    }

    _bulletTexture = SpriteBatchNode::create("bullet_1.png");
    this->addChild(_bulletTexture);
    scheduleUpdate();

    return true;
}

void BulletLayer::addBullet(Vec2 position, DIRECTION direction, GROUP owner, float velocity, BULLETTYPE type) {
    Bullet *bullet = Bullet::create(direction,owner,velocity,type);
    bullet->setPosition(position);
    if (!bullet->initWithTexture(_bulletTexture->getTexture())){
        delete bullet;
        bullet = nullptr;
        return;
    };
    this->addChild(bullet);
    _bulletArray.pushBack(bullet);
}

void BulletLayer::update(float dt) {
    for (auto bullet : _bulletArray){
        switch(bullet->_direction){
            case UP:
                bullet->setPosition(Vec2(bullet->getPositionX(),bullet->getPositionY() + bullet->_velocity));
                break;
            case DOWN:
                bullet->setPosition(Vec2(bullet->getPositionX(),bullet->getPositionY() - bullet->_velocity));
                break;
            case LEFT:
                bullet->setPosition(Vec2(bullet->getPositionX() - bullet->_velocity,bullet->getPositionY()));
                break;
            case RIGHT:
                bullet->setPosition(Vec2(bullet->getPositionX() + bullet->_velocity,bullet->getPositionY()));
                break;
        }
    }
}

BulletLayer::Bullet *BulletLayer::Bullet::create(DIRECTION direction, GROUP owner,
                                                 float velocity, BULLETTYPE type)
{
    Bullet *bullet = new (std::nothrow)Bullet;
    if (bullet && bullet->init(direction,owner,velocity,type)){
        bullet->autorelease();
        return bullet;
    }
    else {
        delete bullet;
        bullet = nullptr;
        return nullptr;
    }
}

bool BulletLayer::Bullet::init(DIRECTION direction, GROUP group, float velocity, BULLETTYPE type) {

    _direction = direction;
    _group = group;
    _velocity = velocity;
    _type = type;

    return true;
}

void BulletLayer::removeBullet(Bullet *bullet) {
    bullet->runAction(Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("blast")),
                                       CallFuncN::create([&](Node *node){
        this->removeChild(node,true);
    }),NULL));
    _bulletArray.eraseObject(bullet,true);
}

Vector<BulletLayer::Bullet *> BulletLayer::getAllBullet() {
    return _bulletArray;
}

GROUP BulletLayer::Bullet::getGroup() {
    return _group;
}
