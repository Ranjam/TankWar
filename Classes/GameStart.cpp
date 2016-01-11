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

    TMXTiledMap *backGround = TMXTiledMap::create("startbackground.tmx");
    backGround->setScale(1.84f);
    backGround->setPosition(Vec2(20,20));
    this->addChild(backGround);

    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/levelstarting.wav");

    MenuItemFont *menuItemFont1 = MenuItemFont::create("Start Game", [](Ref *pSender){
        SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/levelstarting.wav");
        Scene *scene = FightScene::createScene(1,3,3);
        TransitionProgressRadialCW *transitionProgressRadialCW = TransitionProgressRadialCW::create(0.5f,scene);
        Director::getInstance()->replaceScene(transitionProgressRadialCW);
    });
    Menu *menu = Menu::create(menuItemFont1, NULL);
    menu->alignItemsHorizontally();
    menu->setPosition(Vec2(FrameSize.width / 2, FrameSize.height / 2 - 120));
    this->addChild(menu);
    return true;
}
