//
// Created by ranjam on 16-1-6.
//

#ifndef MYGAME_GAMEOVER_H
#define MYGAME_GAMEOVER_H

#include "cocos2d.h"

USING_NS_CC;

class GameOver: public Layer {
public:
    static Scene *createScene();
    CREATE_FUNC(GameOver);
    bool init() override ;
};


#endif //MYGAME_GAMEOVER_H
