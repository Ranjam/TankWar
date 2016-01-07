//
// Created by ranjam on 16-1-6.
//

#include "GameOver.h"
#include "Tool.h"

Scene *GameOver::createScene() {
    Scene *scene = Scene::create();
    GameOver *gameOver = GameOver::create();
    scene->addChild(gameOver);
    return scene;
}

bool GameOver::init() {
    if (!Layer::init()){
        return false;
    }

    Label *label = Label::create("Game Over","Courier",50);
    this->addChild(label);
    label->setPosition(Vec2(FrameSize.width / 2, FrameSize.height / 2));
    return true;
}

