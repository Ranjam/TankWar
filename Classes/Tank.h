//
// Created by ranjam on 15-12-28.
//

#ifndef MYGAME_TANK_H
#define MYGAME_TANK_H

#include "cocos2d.h"
#include "Tool.h"

USING_NS_CC;

class Tank: public Sprite {
public:
    Tank():Sprite(){}
    virtual ~Tank(){}

public:
    enum TANKTYPE {
        P1,P2,ENEMY1,ENEMY2,ENEMY3
    };
    static float p1_velocity;
    static float p2_velocity;
    static float enemy1_velocity;
    static float enemy2_velocity;
    static float enemy3_velocity;

    bool init(TANKTYPE tanktype_value = P1, DIRECTION direction_value = UP, int life_value = 1);

    static Tank *create(TANKTYPE tanktype_value = P1,DIRECTION direction_value = UP);

    DIRECTION getDirection() const;

    void setDirection(DIRECTION direction_value);

    TANKTYPE getTanktype() const;

    char *get_name();

    void setTanktype(TANKTYPE tanktype_value);

    void moveUp();

    void moveDown();

    void moveLeft();

    void moveRight();

    void born();

    void update(float dt) override;

    float getLifeTime();

    bool isBorn();

    int getLife();

    void loseLife();

    void setLife(int life_value);

    void initLife();

    void setFace(DIRECTION direction);

    GROUP getGroup();

    DIRECTION getFace();

private:
    DIRECTION _direction;
    TANKTYPE _type;
    char _tankname[25];
    float _lifetime;
    bool _isborn;
    int _life;
    DIRECTION _faceDirection;
    GROUP _group;
};


#endif //MYGAME_TANK_H
