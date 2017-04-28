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
    addPlayer();
    setCameraTarget();
    return true;
}

void FlappyGame::generateWorld()
{
    _parallaxNode = ParallaxNode::create();
    _parallaxNode->setAnchorPoint(Point::ZERO);

    generateBottomLayer();
    generateMiddleLayer();
    _parallaxNode->addChild(_bottomLayer, 0.0, Vec2(1, 1), Vec2::ZERO);
    _parallaxNode->addChild(_middleLayer, 0.0, Vec2(1, 1), Vec2::ZERO);
  
    this->addChild(_parallaxNode, 1);
    this->schedule(schedule_selector(FlappyGame::updateScene));
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

void FlappyGame::generateMiddleLayer()
{
    _middleLayer = Layer::create();
}

void FlappyGame::addPlayer()
{
    player = PlayerBird::create();
    player->retain();
    player->setPosition(Vec2(windowSize.width/2 - player->getContentSize().width,
                             windowSize.height/2));
    _middleLayer->addChild(player, Z_MIDDLE_LAYER);
}

void FlappyGame::setCameraTarget()
{
    cameraTarget = Follow::create(player, Rect::ZERO);
    cameraTarget->retain();
    this->runAction(cameraTarget);

}

void FlappyGame::updateScene(float dt)
{
    this->updatePlayer(dt);
}

void FlappyGame::updatePlayer(float dt)
{
    player->update(dt);
}
