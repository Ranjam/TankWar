//
// Created by ranjam on 16-1-6.
//

#include "GameStart.h"

Scene *GameStart::createScene() {
    Scene *scene = Scene::create();
    GameStart *gameStartLayer = GameStart::create();
    scene->addChild(gameStartLayer);
    return scene;
}

bool GameStart::init() {
    if (!Layer::init()){
        return false;
    }

    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/levelstarting.wav");

    MenuItemFont *menuItemFont1 = MenuItemFont::create("Start Game", [](Ref *pSender){
        SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/levelstarting.wav");
        Scene *scene = FightScene::createScene(1,3,3);
        TransitionProgressRadialCW *transitionProgressRadialCW = TransitionProgressRadialCW::create(0.5f,scene);
        Director::getInstance()->replaceScene(transitionProgressRadialCW);
    });

    Menu *menu = Menu::create(menuItemFont1, NULL);
    menu->alignItemsHorizontally();
    this->addChild(menu);
    return true;
}
