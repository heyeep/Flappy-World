#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

GameScene::GameScene() {
}

GameScene::~GameScene() {
    CC_SAFE_RELEASE_NULL(player);
}

Scene* GameScene::createScene() {
    Scene* scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -700.0f));

    Layer* layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    this->windowSize = Director::getInstance()->getVisibleSize();
    this->initSprites();
    this->generateWorld();
    this->addPlayer();
    this->initCamera();
    this->initMouseListeners();
    this->initTouchListeners();
    this->initAudio();
    this->stopBackgroundMusic();
    this->preloadAudio();
    this->playBackgroundMusic();

    return true;
}

void GameScene::initSprites() {
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("bird_blue.plist");
}

void GameScene::generateWorld() {
    this->parallaxNode = ParallaxNode::create();

    this->parallaxNode->setAnchorPoint(Point::ZERO);

    this->generateBottomLayer();
    this->generateMiddleLayer();
    this->parallaxNode->addChild(
        this->bottomLayer, 0.0, Vec2(1, 1), Vec2::ZERO);
    this->parallaxNode->addChild(
        this->middleLayer, 0.0, Vec2(1, 1), Vec2::ZERO);

    this->addChild(this->parallaxNode, 1);

    this->schedule(schedule_selector(GameScene::updateScene));
}

void GameScene::generateBottomLayer() {
    this->bottomLayer = Layer::create();
    for (int i = 0; i < 100; i++) {
        this->backgroundImg = Sprite::create("basic_day.png");
        this->backgroundImg->setAnchorPoint(Point::ZERO);
        this->backgroundImg->setPosition(
            i * (this->backgroundImg->getContentSize().width * SCALE_FACTOR),
            0);
        this->backgroundImg->getTexture()->setAliasTexParameters();
        this->backgroundImg->setScale(SCALE_FACTOR);
        this->bottomLayer->addChild(this->backgroundImg, Z_BOTTOM_LAYER);
    }
}

void GameScene::generateMiddleLayer() {
    this->middleLayer = Layer::create();
}

void GameScene::addPlayer() {
    this->player = PlayerBird::create();
    this->player->setPosition(this->getStartingLocation());
    this->middleLayer->addChild(this->player, Z_MIDDLE_LAYER);
}

void GameScene::initCamera() {
    this->cameraObject = Sprite::create();
    this->cameraObject->setPosition(this->getStartingLocation());
    this->setCameraTarget(cameraObject);
    this->middleLayer->addChild(cameraObject);
}

void GameScene::setCameraTarget(cocos2d::Sprite* follow) {
    this->cameraTarget = Follow::create(follow, Rect::ZERO);
    this->runAction(this->cameraTarget);
}

void GameScene::initMouseListeners() {
    this->mouseListener = EventListenerMouse::create();
    this->mouseListener->onMouseDown
        = CC_CALLBACK_1(GameScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        mouseListener, this);
}

void GameScene::initTouchListeners() {
    this->touchListener = EventListenerTouchOneByOne::create();
    this->touchListener->onTouchBegan
        = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    this->touchListener->onTouchEnded
        = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        this->touchListener, this);
}

void GameScene::onMouseDown(cocos2d::Event* event) {
    this->player->flap();
}

Point GameScene::getStartingLocation() {
    Size windowSize = Director::getInstance()->getVisibleSize();
    Point start = Vec2(windowSize.width / 2, windowSize.height / 2);
    return start;
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    return true;
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event*) {
    this->player->flap();
    this->sceneAudio->playEffect("bird_flap.mp3");
}

void GameScene::updateScene(float dt) {
    this->cameraObject->setPositionX(this->player->getPositionX());
    this->updatePlayer(dt);
}

void GameScene::updatePlayer(float dt) {
    this->player->update(dt);
    this->playerDeathCheck();
}

void GameScene::playerDeathCheck() {
    if (this->player->isDead(windowSize)) {
        if (DEBUG_DEATH_ON) {
            this->player->setPositionY(windowSize.height / 2);
            this->player->getPhysicsBody()->setVelocity(Vec2(0, 0));
            this->player->setRotation(0);
        } else {
            this->death();
        }
    }
}

void GameScene::death() {
    Director* director = Director::getInstance();
    Scene* scene = MainMenu::createScene();

    director->replaceScene(scene);
}

void GameScene::initAudio() {
    this->sceneAudio = CocosDenshion::SimpleAudioEngine::getInstance();
}

void GameScene::preloadAudio() {
    this->sceneAudio->preloadBackgroundMusic("game_background.mp3");
    this->sceneAudio->preloadEffect("bird_flap.mp3");
}

void GameScene::playBackgroundMusic() {
    this->sceneAudio->playBackgroundMusic("game_background.mp3");
}

void GameScene::stopBackgroundMusic() {
    this->sceneAudio->stopBackgroundMusic();
}
