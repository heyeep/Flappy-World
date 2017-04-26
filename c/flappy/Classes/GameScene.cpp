#include "GameScene.h"

USING_NS_CC;

Scene* FlappyGame::createScene()
{
    Scene* scene = Scene::create();
    Layer* layer = FlappyGame::create();

    scene->addChild(layer);
    
    return scene;
}

bool FlappyGame::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    windowSize = Director::getInstance()->getVisibleSize();
    generateWorld();

    return true;
}

void FlappyGame::generateWorld()
{
    _parallaxNode = ParallaxNode::create();
    _parallaxNode->setAnchorPoint(Point::ZERO);

    generateBottomLayer();

    _parallaxNode->addChild(_bottomLayer, 0.0, Vec2(1, 1), Vec2::ZERO);
    
    this->addChild(_parallaxNode, 0);
}

void FlappyGame::generateBottomLayer()
{
    _bottomLayer = Layer::create();
    for (int i = 0; i < 100; i++) {
        backgroundImg = Sprite::create("basic_day.png");
        backgroundImg->setAnchorPoint(Point::ZERO);
        backgroundImg->setPosition(i * backgroundImg->getContentSize().width, 0);
        backgroundImg->setScale(SCALE_FACTOR);
        _bottomLayer->addChild(backgroundImg, Z_BOTTOM_LAYER);
    }
}
