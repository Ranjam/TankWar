//
// Created by ranjam on 16-1-3.
//

#ifndef MYGAME_TOOL_H
#define MYGAME_TOOL_H

//#define frameSize Director::getInstance()->getWinSize()
#define winSize Size(480,480)
#define FrameSize Director::getInstance()->getWinSize()

enum DIRECTION {
    UP,DOWN,LEFT,RIGHT,STOP
};

enum GROUP {
    PLAYER, ENEMY
};

#endif //MYGAME_TOOL_H
