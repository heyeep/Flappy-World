#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createScene() {
    // Creates a scene with built in physics.
    Scene* scene = Scene::createWithPhysics();

    // Retrieves the physics world, and gives any object with Physics Enabled a
    // red out. USEFUL
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -600.0f));

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
    this->addCameraObject();
    this->setMouseListeners();
    this->setTouchListeners();
    this->setAudio();
    this->stopBackgroundMusic();
    this->preloadAudio();
    this->playBackgroundMusic();

    return true;
}

/*
  Adds sprites to the cache. Makes it less intensive reloading images.
 */
void GameScene::initSprites()
{
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("bird_blue.plist");
}

/*
  Makes each individual layer and adds it to a ParallaxNode. The node is then added to the Scene.
 */
void GameScene::generateWorld() {
    this->parallaxNode = ParallaxNode::create();

    /*
     Anchor Point is a point that you set as a way to specifying what part
     of the Sprite will be used when setting the position of it.
     (0, 0) - Bottom Left
     (0, 1) - Top Left
     (1, 0) - Bottom Right
     (1, 1) - Top right
     (0.5, 0.5) - Middle
    */
    this->parallaxNode->setAnchorPoint(Point::ZERO);

    // Create each layer separately, add it to the ParallaxNode. The Z level is
    // automatically set to 0.0, It doesn't matter because each layer will be
    // part of the Node.
    this->generateBottomLayer();
    this->generateMiddleLayer();
    this->parallaxNode->addChild(
        this->bottomLayer, 0.0, Vec2(1, 1), Vec2::ZERO);
    this->parallaxNode->addChild(
        this->middleLayer, 0.0, Vec2(1, 1), Vec2::ZERO);

    this->addChild(this->parallaxNode, 1);

    // Start update loop
    this->schedule(schedule_selector(GameScene::updateScene));
}

/*
  Creates the bottom layer (background).
 */
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

/*
  Generates the middle layer. Birds/Objects must be in this layer.
 */
void GameScene::generateMiddleLayer() {
    this->middleLayer = Layer::create();
}

/*
  Creates the user's bird, setting it's location in the middle of the screen,
  this can be changed later.
 */
void GameScene::addPlayer() {
    this->player = PlayerBird::create();
    this->player->setPosition(this->getStartingLocation());
    this->middleLayer->addChild(this->player, Z_MIDDLE_LAYER);
}

/*
  Creates an invisible camera object, the camera follows this instead of the
  player
 */
void GameScene::addCameraObject() {
    this->cameraObject = Sprite::create();
    cameraObject->setPosition(this->getStartingLocation());
    this->setCameraTarget(cameraObject);
    this->middleLayer->addChild(cameraObject);
}

/*
  "Follow" is the Cocos' class/function for cameras. Creates the camera and
  follows the main player. Run action starts the camera.
 */
void GameScene::setCameraTarget(cocos2d::Sprite* follow) {
    this->cameraTarget = Follow::create(follow, Rect::ZERO);
    this->runAction(this->cameraTarget);
}

/*
  Set up game so it recognizes mouse events.
 */
void GameScene::setMouseListeners() {
    this->mouseListener = EventListenerMouse::create();
    this->mouseListener->onMouseDown
        = CC_CALLBACK_1(GameScene::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        mouseListener, this);
}

/*
  Set up game so it recognizes touch events.
 */
void GameScene::setTouchListeners() {
    this->touchListener = EventListenerTouchOneByOne::create();
    this->touchListener->onTouchBegan
        = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    this->touchListener->onTouchEnded
        = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        this->touchListener, this);
}
/*
  Called from setMouseListeners(), makes the bird "flap".
 */
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

/*
  Makes the bird flap after touching the screen.
 */
void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event*) {
    this->player->flap();
    this->sceneAudio->playEffect("bird_flap.mp3");
}

/*
  Updates the position of the camera object, etc
 */
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

void GameScene::setAudio() {
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
