//
// Created by ranjam on 16-1-2.
//

#ifndef MYGAME_BULLET_H
#define MYGAME_BULLET_H

#include "cocos2d.h"
#include "Tool.h"

USING_NS_CC;

class BulletLayer : public Layer{
public:
    enum BULLETTYPE {
        NORMAL, SPECIAL
    };

    class Bullet : public Sprite {
    public:
        bool init(DIRECTION direction, GROUP group, float velocity, BULLETTYPE type);
        static Bullet *create(DIRECTION direction, GROUP group, float velocity, BULLETTYPE type);
        GROUP getGroup();

        DIRECTION _direction;
        GROUP _group;
        float _velocity;
        int _type;
    };

public:

    virtual bool init() override ;
    CREATE_FUNC(BulletLayer);

    void addBullet(Vec2 position, DIRECTION direction = UP, GROUP group = PLAYER,
                   float velocity = 5.0f, BULLETTYPE type = NORMAL);

    void removeBullet(Bullet *bullet);

    Vector<Bullet *> getAllBullet();

    virtual void update(float dt) override ;

private:
    Vector<Bullet *> _bulletArray;
    SpriteBatchNode *_bulletTexture;
};


#endif //MYGAME_BULLET_H
