#include "HelloWorldScene.h"
#include "Tank.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() ) {
        return false;
    }


//    for (int i = 0; i < winSize.width; i += 40){
//        Tank *tank = Tank::create();
//        this->addChild(tank);
//        tank->setPosition(Vec2(i,190));
//        CCLOG("%g",tank->getContentSize().height);
//    }

    int GID[36] = {
            0,0,0,0,1,1,
            0,0,0,0,1,1,
            1,1,1,1,1,1,
            1,1,1,1,1,1,
            1,1,0,0,0,0,
            1,1,0,0,0,0,
    };

    TMXTiledMap *tiledMap = TMXTiledMap::create("Round1.tmx");
    tiledMap->setScale(1.84f);
    this->addChild(tiledMap);

    TMXLayer *tmxLayer = tiledMap->getLayer("layer_0");
    DrawNode *drawNode = DrawNode::create();
    this->addChild(drawNode);

//    Sprite *sprite1 = tmxLayer->getTileAt(Vec2(2, 3));
//    drawNode->drawRect(Vec2(sprite1->getPositionX() * 1.84f,sprite1->getPositionY() * 1.84f),
//                       Vec2(sprite1->getPositionX() * 1.84f + 18.4f, sprite1->getPositionY() * 1.84f + 18.4f),Color4F(1,0,0,1));
//    CCLOG("%g,%g",sprite1->getPositionX(),sprite1->getPositionY() * 1.84f);
    for (int i = 0; i < 26; ++i){
        for (int j = 0; j < 26; ++j){
            Sprite *sprite = tmxLayer->getTileAt(Vec2(i, j));
            if (sprite == nullptr)
                continue;
            if (GID[tmxLayer->getTileGIDAt(Vec2(i, j))])
                drawNode->drawRect(Vec2(sprite->getPositionX() * 1.84f,sprite->getPositionY() * 1.84f),
                                   Vec2(sprite->getPositionX() * 1.84f + 18.4f, sprite->getPositionY() * 1.84f + 18.4f),Color4F(1,0,0,1));
        }
    }

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
