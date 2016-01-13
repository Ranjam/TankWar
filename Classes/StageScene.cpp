//
// Created by ranjam on 16-1-13.
//

#include "StageScene.h"

Scene *StageScene::createScene(int stage, int player1, int player2) {
    Scene *scene = Scene::create();
    StageScene *layer = StageScene::create(stage, player1, player2);
    scene->addChild(layer);
    return scene;
}

bool StageScene::init(int stage, int player1, int player2) {
    if (!LayerColor::init()){
        return false;
    }

    this->initWithColor(Color4B(180, 180, 180, 255));
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/levelstarting.wav");

    char stageLabel[20];
    sprintf(stageLabel, "stage %d", stage);
    Label *label = Label::create(stageLabel,"Courier", 25);
    label->setColor(Color3B(0,0,0));

    MenuItemLabel *menuItemLabel = MenuItemLabel::create(label, [=](Ref *pSender){
        SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/levelstarting.wav");
        Scene *fightScene = FightScene::createScene(stage, player1, player2);
        TransitionProgressRadialCW *transitionProgressRadialCW = TransitionProgressRadialCW::create(0.5f,fightScene);
        Director::getInstance()->replaceScene(transitionProgressRadialCW);
    });
    Menu *menu = Menu::create(menuItemLabel, NULL);
    this->addChild(menu);
    return true;
}

StageScene *StageScene::create(int stage, int player1, int player2) {
    StageScene *stageScene = new (std::nothrow)StageScene;
    if (stageScene && stageScene->init(stage, player1, player2)){
        stageScene->autorelease();
        return stageScene;
    }
    else {
        CC_SAFE_DELETE(stageScene);
        stageScene = nullptr;
        return nullptr;
    }
}

