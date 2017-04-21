#include "MultiPlayerScene.h"

USING_NS_CC;

Scene* MultiPlayerGame::createScene()
{
    auto scene = Scene::create();
    auto layer = MultiPlayerGame::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool MultiPlayerGame::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto gameTitle = Label::createWithSystemFont("MULTI PLAYER", "Arial", 100);
    
    // To position a sprite using it's mid point
    gameTitle->setAnchorPoint(Vec2(0.5, 0.5));
    
    gameTitle->setPosition(Vec2(visibleSize.width/2,
                                visibleSize.height - gameTitle->getContentSize().height));
    
    this->addChild(gameTitle, 1);
    
    return true;
}


