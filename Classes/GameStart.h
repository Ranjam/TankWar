//
// Created by ranjam on 16-1-6.
//

#ifndef MYGAME_GAMESTART_H
#define MYGAME_GAMESTART_H

#include "cocos2d.h"
#include "FightScene.h"

USING_NS_CC;

class GameStart: public Layer {
public:
    static Scene *createScene();
    CREATE_FUNC(GameStart);
    bool init() override ;
};


#endif //MYGAME_GAMESTART_H
