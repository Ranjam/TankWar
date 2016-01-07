//
// Created by ranjam on 15-12-28.
//

#include "FightScene.h"

bool FightScene::init(int stage, int player1, int player2) {
    if(!Layer::init()) {
        return false;
    }

    //AudioEngine init
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/fexplosion.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/shoot.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/moving.wav");

    //Bullet layer init
    _bulletLayer = BulletLayer::create();
    this->addChild(_bulletLayer);
    _bulletLayer->retain();

    //TileMap
    _tiledMap = TMXTiledMap::create("Round1.tmx");
    this->addChild(_tiledMap);
    _tiledMap->retain();
    _tiledMap->setScale(1.84f);

    //Set the tank under the tank
    TMXLayer *tmxLayer = _tiledMap->getLayer("layer_1");
    tmxLayer->setGlobalZOrder(3);

    //Animation init
    //born
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("born.plist");
    Animation *born = Animation::create();
    for (int i = 1; i < 5; ++i){
        char name[20];
        sprintf(name,"born%d.png",i);
        born->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
    }
    born->setDelayPerUnit(0.15f);
    born->setRestoreOriginalFrame(true);
    born->setLoops(3);
    AnimationCache::getInstance()->addAnimation(born,"born");

    //died
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("blast.plist");
    Animation *died = Animation::create();
    for (int i = 1; i < 9; ++i){
        char name[20];
        sprintf(name,"blast%d.gif",i);
        died->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
    }
    died->setDelayPerUnit(0.05f);
    died->setRestoreOriginalFrame(false);
    died->setLoops(1);
    AnimationCache::getInstance()->addAnimation(died,"died");

    //Bullet died
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("blast.plist");
    Animation *blast = Animation::create();
    for (int i = 1; i < 3; ++i){
        char name[20];
        sprintf(name,"blast%d.gif",i);
        blast->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
    }
    blast->setDelayPerUnit(0.05f);
    blast->setRestoreOriginalFrame(false);
    blast->setLoops(1);
    AnimationCache::getInstance()->addAnimation(blast,"blast");

    //init player life and set symbol
    this->_player1 = player1;
    this->_player2 = player2;
    _symbol = 1;
    Sprite *symbol = Sprite::create("symbol.png");
    this->addChild(symbol);
    symbol->setPosition(Vec2(240, 20));
    symbol->setTag(999);

    setPlayer();

    //Open the schedule update
    this->scheduleUpdate();
    this->schedule(schedule_selector(FightScene::enemyTankMove), 0, -1, 1.65f);
    this->schedule(schedule_selector(FightScene::addEnemy), 1.0f);
    return true;
}

// Tank control function
void FightScene::tankControl_p1(EventKeyboard::KeyCode keyCode, Event *event) {
    auto target = event->getCurrentTarget();
    auto p1 = static_cast<Tank *>(target);

    switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
                p1->setDirection(DIRECTION::UP);
                break;
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                p1->setDirection(DIRECTION::DOWN);
                break;
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                p1->setDirection(DIRECTION::LEFT);
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                p1->setDirection(DIRECTION::RIGHT);
                break;
            case EventKeyboard::KeyCode::KEY_KP_ENTER:
                SimpleAudioEngine::getInstance()->playEffect("sounds/shoot.wav");
                _bulletLayer->addBullet(Vec2(p1->getPositionX(),p1->getPositionY()),p1->getFace());
                break;
    }
}

void FightScene::tankControl_p2(EventKeyboard::KeyCode keyCode, Event *event) {
    auto target = event->getCurrentTarget();
    auto tank = static_cast<Tank *>(target);
    switch (keyCode) {
            case EventKeyboard::KeyCode::KEY_W:
                tank->setDirection(DIRECTION::UP);
                break;
            case EventKeyboard::KeyCode::KEY_S:
                tank->setDirection(DIRECTION::DOWN);
                break;
            case EventKeyboard::KeyCode::KEY_A:
                tank->setDirection(DIRECTION::LEFT);
                break;
            case EventKeyboard::KeyCode::KEY_D:
                tank->setDirection(DIRECTION::RIGHT);
                break;
            case EventKeyboard::KeyCode::KEY_SPACE:
                SimpleAudioEngine::getInstance()->playEffect("sounds/shoot.wav");
                _bulletLayer->addBullet(Vec2(tank->getPositionX(),tank->getPositionY()),tank->getFace());
                break;
    }
}

void FightScene::update(float dt) {

    setPlayer();
    controlPlayer();
//    autoDead();  // just for debug
    adjustBullet();
    removeTank();
    gameOverCheck();
}

// No use
void FightScene::onEnter() {
    Node::onEnter();
}

//Enemy tank move func
void FightScene::enemyTankMove(float dt) {

    static int moveHZ1 = 0;
    static int moveHZ2 = 0;
    static int moveHZ3 = 0;

    moveHZ1 = (moveHZ1 + 1) % 60;
    moveHZ2 = (moveHZ2 + 1) % 40;
    moveHZ3 = (moveHZ3 + 1) % 80;

    DIRECTION direction[5] = {
            DIRECTION::UP,
            DIRECTION::DOWN,
            DIRECTION::LEFT,
            DIRECTION::RIGHT,
            DIRECTION::STOP
    };

    for(auto tank: _enemyArray){
        //Can't move while borning
        if (tank->isBorn())
            continue;
        DIRECTION direct = direction[(int)(CCRANDOM_0_1() * 4)];
        if (tank->getTanktype() == Tank::TANKTYPE::ENEMY1 && moveHZ1 % 50 == 0
            || tank->getTanktype() == Tank::TANKTYPE::ENEMY2 && moveHZ2 % 30 == 0
            || tank->getTanktype() == Tank::TANKTYPE::ENEMY3 && moveHZ3 % 70 == 0 ) {
            tank->stopAllActions();
            switch (direct) {
                case DIRECTION::UP:
                    tank->setDirection(DIRECTION::UP);
                    break;
                case DIRECTION::DOWN:
                    tank->setDirection(DIRECTION::DOWN);
                    break;
                case DIRECTION::LEFT:
                    tank->setDirection(DIRECTION::LEFT);
                    break;
                case DIRECTION::RIGHT:
                    tank->setDirection(DIRECTION::RIGHT);
                    break;
                case DIRECTION::STOP:
                    break;
            }
            SimpleAudioEngine::getInstance()->playEffect("sounds/shoot.wav");
            _bulletLayer->addBullet(Vec2(tank->getPositionX(),tank->getPositionY()),tank->getDirection(),tank->getGroup());
        }

//        if ((int)tank->getLifeTime() % 5 == 0) {
//            _bulletLayer->addBullet(Vec2(tank->getPositionX(),tank->getPositionY()),tank->getDirection(),tank->getGroup());
//        }

        switch (tank->getDirection()){
            case UP:
                if (canMove(tank,DIRECTION::UP))
                    tank->moveUp();
                break;
            case DOWN:
                if (canMove(tank,DIRECTION::DOWN))
                    tank->moveDown();
                break;
            case LEFT:
                if (canMove(tank,DIRECTION::LEFT))
                    tank->moveLeft();
                break;
            case RIGHT:
                if (canMove(tank,DIRECTION::RIGHT))
                    tank->moveRight();
                break;
            case STOP:
                break;
        }
    }
}

//Check tank if can move
bool FightScene::canMove(Tank *tank, DIRECTION move_direction) {
    Rect tankRect = tank->getBoundingBox();
    Vec2 judgePoint[3];
    switch (move_direction) {
        case DIRECTION::UP:
            judgePoint[0] = Vec2(tankRect.getMinX() + 5,tankRect.getMaxY() - 5);
            judgePoint[1] = Vec2(tankRect.getMaxX() - 5,tankRect.getMaxY() - 5);
            judgePoint[2] = Vec2(tankRect.getMidX() ,tankRect.getMaxY() - 5);
            break;
        case DIRECTION::DOWN:
            judgePoint[0] = Vec2(tankRect.getMinX() + 5,tankRect.getMinY() + 5);
            judgePoint[1] = Vec2(tankRect.getMaxX() - 5,tankRect.getMinY() + 5);
            judgePoint[2] = Vec2(tankRect.getMidX(), tankRect.getMinY() + 5);
            break;
        case DIRECTION::LEFT:
            judgePoint[0] = Vec2(tankRect.getMinX() + 5,tankRect.getMinY() + 5);
            judgePoint[1] = Vec2(tankRect.getMinX() + 5,tankRect.getMaxY() - 5);
            judgePoint[2] = Vec2(tankRect.getMinX() + 5,tankRect.getMidY());
            break;
        case DIRECTION::RIGHT:
            judgePoint[0] = Vec2(tankRect.getMaxX() - 5,tankRect.getMinY() + 5);
            judgePoint[1] = Vec2(tankRect.getMaxX() - 5,tankRect.getMaxY() - 5);
            judgePoint[2] = Vec2(tankRect.getMaxX() - 5,tankRect.getMidY());
            break;
        case DIRECTION::STOP:
            break;
    }

    //Tank overlap check
    for(auto tank_temp = _allTankArray.begin(); tank_temp != _allTankArray.end(); ++tank_temp){
        //Borned and self pass
        if ((*tank_temp) == tank || (*tank_temp)->isBorn())
            continue;
        if ((*tank_temp)->getBoundingBox().containsPoint(judgePoint[0]) ||
                (*tank_temp)->getBoundingBox().containsPoint(judgePoint[1])){
            return false;
        }
    }

    //Edge check
    if (judgePoint[2].x < 0 || judgePoint[2].x > winSize.width - 0
        || judgePoint[2].y < 0 || judgePoint[2].y > winSize.height - 0)
        return false;

    //Bullet check
    for (auto tank : _allTankArray){
        for (auto bullet : _bulletLayer->getAllBullet()){
            if (tank->getBoundingBox().intersectsRect(bullet->getBoundingBox())
                && bullet->getGroup() != tank->getGroup()){
                tank->loseLife();
                _bulletLayer->removeBullet(bullet);
            }
        }
    }

    //Walls check
    TMXLayer *tmxLayer0 = _tiledMap->getLayer("layer_0");

    int GID[36] = {
            0,0,0,0,1,1,
            0,0,0,0,1,1,
            1,1,1,1,1,1,
            1,1,1,1,1,1,
            1,1,0,0,0,0,
            1,1,0,0,0,0,
    };

    DrawNode *drawNode = DrawNode::create();
    this->addChild(drawNode);
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            Sprite *sprite = tmxLayer0->getTileAt(Vec2(i, j));
            if (sprite == NULL) {
                continue;
            }
            // Debug drawnode
//            if (GID[tmxLayer0->getTileGIDAt(Vec2(i, j))])
//                drawNode->drawRect(Vec2(sprite->getPositionX() * 1.84f,sprite->getPositionY() * 1.84f),
//                                   Vec2(sprite->getPositionX() * 1.84f + 18.4f, sprite->getPositionY() * 1.84f + 18.4f),Color4F(1,0,0,1));
//            drawNode->drawPoint(judgePoint[0],1,Color4F(1,0,0,1));
//            drawNode->drawPoint(judgePoint[1],1,Color4F(1,0,0,1));
//            drawNode->drawPoint(judgePoint[2],1,Color4F(1,0,0,1));
            int gid = tmxLayer0->getTileGIDAt(Vec2(i, j));
            Rect tileRect = Rect(sprite->getPositionX() * 1.84f, sprite->getPositionY() * 1.84f, 18.4f, 18.4f);
            if ((tileRect.containsPoint(judgePoint[0]) || tileRect.containsPoint(judgePoint[1])
                || tileRect.containsPoint(judgePoint[2])) && GID[gid - 1]) {
                return false;
            }
        }
    }

    if (1)
        return true;
}

//Add enemy func
void FightScene::addEnemy(float dt) {
    static int enemyBornHZ = 0;
    enemyBornHZ = (enemyBornHZ + 1) % (ENEMYBORNHZ + 1);
    Vec2 bornPlace[3] = {
            Vec2(20,winSize.height - 20),Vec2(winSize.width / 2, winSize.height - 20),Vec2(winSize.width - 20, winSize.height - 20)
    };
    Tank::TANKTYPE tanktype[3] = {
            Tank::TANKTYPE::ENEMY1, Tank::TANKTYPE::ENEMY2, Tank::TANKTYPE::ENEMY3,
    };
    int randomPlace = (int)(CCRANDOM_0_1()*3);
    int randomType = (int)(CCRANDOM_0_1()*3);
    if (_enemyArray.size() < 3 || enemyBornHZ % ENEMYBORNHZ == 0 && _enemyArray.size() < ENEMYMAXCOUNT){
        Tank *tank = Tank::create(tanktype[randomType],DIRECTION::DOWN);
        tank->setPosition(bornPlace[randomPlace]);
        this->addChild(tank);
        tank->born();
        _enemyArray.pushBack(tank);
        _allTankArray.pushBack(tank);
    }
}

//Remove tank func
void FightScene::removeTank() {

    for (auto tank : _allTankArray){
        if (tank->getLife() <= 0){
            _allTankArray.eraseObject(tank);
            _enemyArray.eraseObject(tank);
            if (tank == (Tank *)getChildByTag(1)){
                _eventDispatcher->pauseEventListenersForTarget(tank);
                --_player1;
            }
            if (tank == (Tank *)getChildByTag(2)){
                _eventDispatcher->pauseEventListenersForTarget(tank);
                --_player2;
            }
            SimpleAudioEngine::getInstance()->playEffect("sounds/fexplosion.wav");
            tank->runAction(Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("died")),
                                             CallFuncN::create([&](Node *tankNode){
                this->removeChild(tankNode,true);
            }),NULL));
        }
    }
}

// Debug func
void FightScene::autoDead() {

    static int deadline = 0;
    deadline = (deadline + 1) % 501;

    for(auto tank : _allTankArray){
        if (deadline == 500){
            tank->loseLife();
        }
    }
}

// Control player func
void FightScene::controlPlayer() {

    //Get tank
    auto p1 = static_cast<Tank *>(this->getChildByTag(1));
    auto p2 = static_cast<Tank *>(this->getChildByTag(2));

    if (p1 != nullptr) {
        //Born
        if (p1->isBorn() || p1->getLife() <= 0) {
            _eventDispatcher->pauseEventListenersForTarget(p1);
        }
        else {
            _eventDispatcher->resumeEventListenersForTarget(p1);

            //Control p1 & p2
            switch (p1->getDirection()) {
                case DIRECTION::UP:
                    if (canMove(p1, DIRECTION::UP))
                        p1->moveUp();
                    break;
                case DIRECTION::DOWN:
                    if (canMove(p1, DIRECTION::DOWN))
                        p1->moveDown();
                    break;
                case DIRECTION::LEFT:
                    if (canMove(p1, DIRECTION::LEFT))
                        p1->moveLeft();
                    break;
                case DIRECTION::RIGHT:
                    if (canMove(p1, DIRECTION::RIGHT))
                        p1->moveRight();
                    break;
                case DIRECTION::STOP:
                    break;
            }
        }
    }

    if (p2 != nullptr) {
        if (p2->isBorn() || p2->getLife() <= 0) {
            _eventDispatcher->pauseEventListenersForTarget(p2);
        }
        else {
            _eventDispatcher->resumeEventListenersForTarget(p2);

            switch (p2->getDirection()) {
                case DIRECTION::UP:
                    if (canMove(p2, DIRECTION::UP))
                        p2->moveUp();
                    break;
                case DIRECTION::DOWN:
                    if (canMove(p2, DIRECTION::DOWN))
                        p2->moveDown();
                    break;
                case DIRECTION::LEFT:
                    if (canMove(p2, DIRECTION::LEFT))
                        p2->moveLeft();
                    break;
                case DIRECTION::RIGHT:
                    if (canMove(p2, DIRECTION::RIGHT))
                        p2->moveRight();
                    break;
                case DIRECTION::STOP:
                    break;
            }
        }
    }
}

//Set player func
void FightScene::setPlayer() {
    //Create tank sprite
    if (_player1 && this->getChildByTag(1) == nullptr) {
        auto p1 = Tank::create();
        this->addChild(p1);
        p1->setPosition(Vec2(180, 20));
        p1->setTag(1);
        p1->setDirection(DIRECTION::STOP);
        _allTankArray.pushBack(p1);
        p1->born();

        //Create event listener
        EventListenerKeyboard *tankControlEventlistener_p1 = EventListenerKeyboard::create();
        tankControlEventlistener_p1->onKeyPressed = CC_CALLBACK_2(FightScene::tankControl_p1, this);
        tankControlEventlistener_p1->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event *event) {
            switch (keyCode) {
                case EventKeyboard::KeyCode::KEY_UP_ARROW:
                case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
                case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
                    p1->setDirection(DIRECTION::STOP);
            }
        };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(tankControlEventlistener_p1,p1);
    }

    if (_player2 && this->getChildByTag(2) == nullptr) {
        auto p2 = Tank::create(Tank::TANKTYPE::P2);
        this->addChild(p2);
        p2->setPosition(Vec2(300, 20));
        p2->setTag(2);
        p2->setDirection(DIRECTION::STOP);
        _allTankArray.pushBack(p2);
        p2->born();

        EventListenerKeyboard *tankControlEventlistener_p2 = EventListenerKeyboard::create();
        tankControlEventlistener_p2->setEnabled(true);
        tankControlEventlistener_p2->onKeyPressed = CC_CALLBACK_2(FightScene::tankControl_p2, this);
        tankControlEventlistener_p2->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event *event) {
            switch (keyCode) {
                case EventKeyboard::KeyCode::KEY_W:
                case EventKeyboard::KeyCode::KEY_S:
                case EventKeyboard::KeyCode::KEY_D:
                case EventKeyboard::KeyCode::KEY_A:
                    p2->setDirection(DIRECTION::STOP);
            }
        };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(tankControlEventlistener_p2,p2);
    }
}

//Create fight layer
FightScene *FightScene::create(int stage, int player1, int player2) {
    FightScene *layer = new (std::nothrow)FightScene;
    if (layer && layer->init(stage,player1,player2)){
        layer->autorelease();
        return layer;
    }
    else {
        delete layer;
        layer = nullptr;
        return nullptr;
    }
}

//Create fight layer
Scene *FightScene::createScene(int stage, int player1, int player2) {
    Scene *scene = Scene::create();
    FightScene *fightScene = FightScene::create(stage,player1,player2);
    scene->addChild(fightScene);
    return scene;
}

//Adjust bullets
void FightScene::adjustBullet() {
    for (auto bullet : _bulletLayer->getAllBullet()){
        if (bullet->getPositionX() < 0 || bullet->getPositionX() > winSize.width
                || bullet->getPositionY() < 0 || bullet->getPositionY() > winSize.height){
            _bulletLayer->removeBullet(bullet);
        }
        for (auto bullet_other : _bulletLayer->getAllBullet()){
            if (bullet->getBoundingBox().intersectsRect(bullet_other->getBoundingBox())
                && bullet->getGroup() != bullet_other->getGroup()){
                _bulletLayer->removeBullet(bullet);
                _bulletLayer->removeBullet(bullet_other);
            }
        }

        int GID[36] = {
            0,0,0,0,2,2,
            0,0,0,0,2,2,
            1,1,1,1,3,3,
            1,1,1,1,3,3,
            3,3,0,0,0,0,
            3,3,0,0,0,0,
        };

        TMXLayer *tmxLayer0 = _tiledMap->getLayer("layer_0");
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 26; ++j) {
                Sprite *sprite = tmxLayer0->getTileAt(Vec2(i, j));
                if (sprite == nullptr)
                    continue;
                Rect tileRect = Rect(sprite->getPositionX() * 1.84f, sprite->getPositionY() * 1.84f, 18.4f, 18.4f);
                if (tileRect.intersectsRect(bullet->getBoundingBox())) {
                    if (GID[tmxLayer0->getTileGIDAt(Vec2(i, j)) - 1] == 1) {
                        tmxLayer0->setTileGID(1, Vec2(i, j));
                        _bulletLayer->removeBullet(bullet);
                    }
                    else if (GID[tmxLayer0->getTileGIDAt(Vec2(i, j)) - 1] == 2) {
                        _bulletLayer->removeBullet(bullet);
                    }
                    else if (GID[tmxLayer0->getTileGIDAt(Vec2(i, j)) - 1] == 3) {
                        _bulletLayer->removeBullet(bullet);
                        Sprite *symbol = static_cast<Sprite *>(this->getChildByTag(999));
                        if (symbol == nullptr)
                            continue;
                        symbol->runAction(Sequence::create(Animate::create(AnimationCache::getInstance()->getAnimation("died")),
                                          CallFuncN::create([&](Node *node){ this->removeChild(node,true); }), NULL));
                        _symbol = 0;
                        tmxLayer0->setTileGID(25, Vec2(12, 24));
                        tmxLayer0->setTileGID(31, Vec2(12, 25));
                        tmxLayer0->setTileGID(26, Vec2(13, 24));
                        tmxLayer0->setTileGID(32, Vec2(13, 25));
                    }
                }
            }
        }
    }
}

// Check gameover func
void FightScene::gameOverCheck() {
    if (_player1 == 0 && _player2 == 0 || _symbol == 0){
        SimpleAudioEngine::getInstance()->end();
        this->unscheduleAllCallbacks();
        Sprite *gameover = Sprite::create("over.png");
        this->addChild(gameover);
        gameover->setScale(2.0f);
        gameover->setPosition(Vec2(winSize.width / 2, 0));
        gameover->runAction(Sequence::create(MoveTo::create(2,Vec2(winSize.width / 2, winSize.height / 2)),
                                             Blink::create(3,3),
                                             CallFunc::create([](){
                                                 Director::getInstance()->replaceScene(GameOver::createScene());
                                             }),NULL));
    }
}

