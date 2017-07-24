#include "GameScene.h"
#include "Coin.h"
#include "CoordinateUpdate.h"
#include "Network.h"
#include "Points.h"
#include "SimpleAudioEngine.h"
#include <list>

USING_NS_CC;

GameScene::GameScene() {
}

GameScene::~GameScene() {
    Network::getInstance()->unsubscribe(COORDINATES_UPDATE_KEY, this);
    CC_SAFE_RELEASE_NULL(player);
}

Scene* GameScene::createScene(std::list<Pipe*> pipes) {
    GameScene* layer = GameScene::create();
    layer->loadStage(pipes);
    layer->initCollisionDetectionSystem();

    Scene* scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -700.0f));

    scene->addChild(layer);

    return scene;
}

Scene* GameScene::createScene() {
    return createScene(std::list<Pipe*>());
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
    if (DEBUG_ENABLE_MUSIC_ON) {
        this->playBackgroundMusic();
    }

    Network::getInstance()->subscribe(COORDINATES_UPDATE_KEY,
        this,
        [this](bool success, nlohmann::json json) {
            CoordinateUpdate update = CoordinateUpdate::fromPayload(json);
            for (PlayerBird* p : this->players) {
                int playerServerId = p->getServerId();
                if (playerServerId == update.serverId) {
                    p->getPhysicsBody()->setPositionOffset(
                        Vec2(update.x, update.y));
                    p->getPhysicsBody()->setVelocity(
                        Vec2(update.vx, update.vy));
                }
            }
        });

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

void GameScene::loadStage(std::list<Pipe*> pipes) {
    for (Pipe* p : pipes) {
        p->initPipe();
        this->middleLayer->addChild(p, Z_MIDDLE_LAYER);
    }
    initPointsFromPipes(pipes);
    initCoinFromPipes(pipes);
}

void GameScene::initPointsFromPipes(std::list<Pipe*> pipes) {
    std::list<float> xCoords;
    for (Pipe* p : pipes) {
        bool xCoordInList
            = std::find(std::begin(xCoords), std::end(xCoords), p->getXPos())
            != std::end(xCoords);
        if (!xCoordInList) {
            xCoords.push_back(p->getXPos());
        }
    }
    for (float xCoord : xCoords) {
        Points* points = Points::create(1, xCoord);
        points->initPoints();
        this->middleLayer->addChild(points, Z_MIDDLE_LAYER);
    }
}

void GameScene::initCoinFromPipes(std::list<Pipe*> pipes) {
    std::vector<float> xCoords;
    float hSize = windowSize.height / 8;
    for (Pipe* p : pipes) {
        bool xCoordInList
            = std::find(std::begin(xCoords), std::end(xCoords), p->getXPos())
            != std::end(xCoords);
        if (!xCoordInList) {
            xCoords.push_back(p->getXPos());
        }
    }
    // Starts at 1 to avoid making a coin in front of the first pipe.
    for (int i = 1; i < xCoords.size(); i++) {
        float xPos = xCoords[i] - ((xCoords[i] - xCoords[i - 1]) / 2);
        Coin* coin
            = Coin::create(xPos, RandomHelper::random_int(hSize, hSize * 4));
        coin->initCoin();
        this->middleLayer->addChild(coin, Z_MIDDLE_LAYER + 1);
    }
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
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        mouseListener, this);
}

void GameScene::initTouchListeners() {
    this->touchListener = EventListenerTouchOneByOne::create();
    this->touchListener->onTouchBegan
        = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    this->touchListener->onTouchEnded
        = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        this->touchListener, this);
}

void GameScene::onMouseDown(cocos2d::Event* event) {
    if (this->gameOver) {
        this->gotoMainMenu();
    }
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
    this->sceneAudio->playEffect("bird_flap.wav");
}

void GameScene::updateScene(float dt) {
    this->cameraObject->setPositionX(this->player->getPositionX());
    this->updatePlayer(dt);

    std::shared_ptr<CoordinateUpdate> update
        = std::make_shared<CoordinateUpdate>(this->player->getServerId(),
            this->player->getPositionX(),
            this->player->getPositionY(),
            this->player->getPhysicsBody()->getVelocity().x,
            this->player->getPhysicsBody()->getVelocity().y);
    Network::getInstance()->updateServer(update);
}

void GameScene::updatePlayer(float dt) {
    this->player->update(dt);
    this->playerDeathCheck();
}

void GameScene::playerDeathCheck() {
    if (!gameOver) {
        if (this->player->isDead(windowSize)) {
            this->sceneAudio->playEffect("bird_hit.wav");
            if (DEBUG_DEATH_ON) {
                this->player->setPositionY(windowSize.height / 2);
                this->player->getPhysicsBody()->setVelocity(Vec2(0, 0));
                this->player->setRotation(0);
            } else {
                this->death();
            }
        }
    }
}

void GameScene::death() {
    this->gameOver = true;
    this->player->dead = true;
    this->player->getPhysicsBody()->setVelocity(Vec2(0, 0));
    this->player->getPhysicsBody()->setGravityEnable(false);
    this->player->getPhysicsBody()->setDynamic(false);
    this->displayScore();
}

void GameScene::initAudio() {
    this->sceneAudio = CocosDenshion::SimpleAudioEngine::getInstance();
}

void GameScene::preloadAudio() {
    this->sceneAudio->preloadBackgroundMusic("game_background.wav");
    this->sceneAudio->preloadEffect("bird_flap.wav");
    this->sceneAudio->preloadEffect("bird_hit.wav");
}

void GameScene::playBackgroundMusic() {
    this->sceneAudio->playBackgroundMusic("game_background.wav");
}

void GameScene::stopBackgroundMusic() {
    this->sceneAudio->stopBackgroundMusic();
}

void GameScene::gotoMainMenu() {
    Director* director = Director::getInstance();
    Scene* scene = MainMenu::createScene();

    director->replaceScene(scene);
}

void GameScene::displayScore() {
    Sprite* scoreBackground = Sprite::create("endgame_scores.png");
    scoreBackground->setAnchorPoint(Vec2(0.5, 0.5));
    scoreBackground->setPosition(
        Vec2(this->player->getPositionX(), windowSize.height / 1.5));
    scoreBackground->getTexture()->setAliasTexParameters();
    scoreBackground->setScale(SCALE_FACTOR);
    this->middleLayer->addChild(scoreBackground, 1);

    std::string points = std::to_string(this->points);
    Label* pointsTitle
        = Label::createWithTTF("Points: " + points, "fonts/flappy.TTF", 50);
    pointsTitle->setAnchorPoint(Point::ZERO);
    pointsTitle->setPosition(Vec2(scoreBackground->getPositionX()
            - (((scoreBackground->getContentSize().width / 2) * SCALE_FACTOR)
                  - (pointsTitle->getContentSize().width / 5)),
        (scoreBackground->getPositionY()
            + (scoreBackground->getContentSize().height / 2))));
    pointsTitle->enableShadow(Color4B(100, 100, 100, 100), Size(-3, -3), 0);
    pointsTitle->enableOutline(Color4B(25, 25, 25, 255), 3);
    pointsTitle->setColor(Color3B(255, 255, 255));
    this->middleLayer->addChild(pointsTitle, 1);

    std::string coins = std::to_string(this->coins);
    Label* coinsTitle
        = Label::createWithTTF("Coins: " + coins, "fonts/flappy.TTF", 50);
    coinsTitle->setAnchorPoint(Vec2(0, 1));
    coinsTitle->setPosition(Vec2(scoreBackground->getPositionX()
            - (((scoreBackground->getContentSize().width / 2) * SCALE_FACTOR)
                  - (pointsTitle->getContentSize().width / 5)),
        (scoreBackground->getPositionY()
            - (scoreBackground->getContentSize().height / 2))));
    coinsTitle->enableShadow(Color4B(100, 100, 100, 100), Size(-3, -3), 0);
    coinsTitle->enableOutline(Color4B(25, 25, 25, 255), 3);
    coinsTitle->setColor(Color3B(255, 255, 255));
    this->middleLayer->addChild(coinsTitle, 1);
}

void GameScene::initCollisionDetectionSystem() {
    EventListenerPhysicsContact* contactListener
        = EventListenerPhysicsContact::create();
    contactListener->onContactBegin
        = CC_CALLBACK_1(GameScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        contactListener, this);
}

bool GameScene::onContactBegin(cocos2d::PhysicsContact& contact) {
    PhysicsBody* bodyA = contact.getShapeA()->getBody();
    PhysicsBody* bodyB = contact.getShapeB()->getBody();
    // Collision for dense objects
    if ((bodyA->getCategoryBitmask() & bodyB->getCollisionBitmask())
            == BITMASK_COLLISION_DENSE
        || (bodyB->getCategoryBitmask() & bodyA->getCollisionBitmask())
            == BITMASK_COLLISION_DENSE) {
        // Player <--> Pipes
        if ((bodyA->getTag() == TAG_PLAYER && bodyB->getTag() == TAG_PIPE)
            || (bodyA->getTag() == TAG_PIPE && bodyB->getTag() == TAG_PLAYER)) {
            CCLOG("Collision Detected: Pipes");
            this->death();
        }
        // Passable objects
    } else if ((bodyA->getCategoryBitmask() & bodyB->getCollisionBitmask())
            == !BITMASK_COLLISION_DENSE
        || (bodyB->getCategoryBitmask() & bodyA->getCollisionBitmask())
            == !BITMASK_COLLISION_DENSE) {
        // Player <--> Point
        if ((bodyA->getTag() == TAG_PLAYER && bodyB->getTag() == TAG_POINTS)
            || (bodyA->getTag() == TAG_POINTS
                   && bodyB->getTag() == TAG_PLAYER)) {
            CCLOG("Collision Detected: Points");
            Node* pointN;
            if (bodyA->getTag() == TAG_POINTS)
                pointN = bodyA->getNode();
            if (bodyB->getTag() == TAG_POINTS)
                pointN = bodyB->getNode();
            Points* pointP = dynamic_cast<Points*>(pointN);
            this->points += pointP->getValue();
            // Player <--> Coin
        } else if ((bodyA->getTag() == TAG_PLAYER
                       && bodyB->getTag() == TAG_COIN)
            || (bodyA->getTag() == TAG_COIN && bodyB->getTag() == TAG_PLAYER)) {
            CCLOG("Collision Detected: Coins");
            Node* coinN;
            if (bodyA->getTag() == TAG_COIN)
                coinN = bodyA->getNode();
            if (bodyB->getTag() == TAG_COIN)
                coinN = bodyB->getNode();
            Coin* coinP = dynamic_cast<Coin*>(coinN);
            this->coins += coinP->getValue();
            coinP->removeFromParent();
        }
    }

    return true;
}
