//
// Created by ranjam on 15-12-28.
//

#ifndef MYGAME_FIGHTSCENE_H
#define MYGAME_FIGHTSCENE_H

#include "cocos2d.h"
#include "Tank.h"
#include "BulletLayer.h"
#include "GameOver.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

#define ENEMYMAXCOUNT 5
#define ENEMYBORNHZ 10

class FightScene : public Layer{
public:
    bool init(int stage,int player1,int player2);
    static FightScene * create(int stage, int player1, int player2);
    static Scene *createScene(int stage, int player1, int player2);
public:
    void tankControl_p1(EventKeyboard::KeyCode keyCode, Event *event);
    void tankControl_p2(EventKeyboard::KeyCode keyCode, Event *event);

    void update(float dt) override ;

    void onEnter() override ;

    void setPlayer();

    void controlPlayer();

    void enemyTankMove(float dt);

    void addEnemy(float dt);

    void gameOverCheck();

    bool canMove(Tank *tank, DIRECTION move_direction);

    void removeTank();

    void adjustBullet();

    void autoDead();

    void winTheScene();

private:
    Vector<Tank *> _enemyArray;
    Vector<Tank *> _allTankArray;
    int _player1;
    int _player2;
    int _symbol;
    int _enemyCount;
    int _currentStage;
    BulletLayer *_bulletLayer;
    TMXTiledMap *_tiledMap;
};


#endif //MYGAME_TESTLAYER01_H
