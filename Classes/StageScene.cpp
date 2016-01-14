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
    _stage = stage;

    char stageLabel[20];
    sprintf(stageLabel, "stage %d", stage);
    Label *label = Label::create(stageLabel,"Courier", 25);
    label->setColor(Color3B(0,0,0));

    MenuItemLabel *menuItemLabel = MenuItemLabel::create(label, [=](Ref *pSender){
        SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/levelstarting.wav");
        Scene *fightScene = FightScene::createScene(_stage, player1, player2);
        TransitionProgressRadialCW *transitionProgressRadialCW = TransitionProgressRadialCW::create(0.5f,fightScene);
        Director::getInstance()->replaceScene(transitionProgressRadialCW);
    });
    label->setTag(1);
    menuItemLabel->setTag(2);
    Menu *menu = Menu::create(menuItemLabel, NULL);
    menu->setTag(3);
    this->addChild(menu);

    EventListenerKeyboard *eventListenerKeyboard = EventListenerKeyboard::create();
    eventListenerKeyboard->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event *event){
        if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW){
            _stage++;
            if (_stage > 20){
                _stage = 1;
            }
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW){
            _stage--;
            if (_stage < 1){
                _stage = 20;
            }
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(eventListenerKeyboard, this);
    scheduleUpdate();
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

void StageScene::update(float dt) {
    Menu *menu = (Menu *)this->getChildByTag(3);
    MenuItemLabel *itemLabel = (MenuItemLabel *)menu->getChildByTag(2);
    Label *label1 = (Label *)itemLabel->getChildByTag(1);
    char str[20];
    sprintf(str, "stage %d", this->_stage);
    label1->setString(str);
}
