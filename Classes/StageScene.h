//
// Created by ranjam on 16-1-13.
//

#ifndef MYGAME_STAGESCENE_H
#define MYGAME_STAGESCENE_H

#include "cocos2d.h"
#include "FightScene.h"
#include "Tool.h"

USING_NS_CC;

class StageScene: public LayerColor {
public:
    static Scene* createScene(int stage, int player1, int player2);
    static StageScene *create(int stage, int player1, int player2);
    bool init(int stage, int player1, int player2);
};


#endif //MYGAME_STAGESCENE_H
