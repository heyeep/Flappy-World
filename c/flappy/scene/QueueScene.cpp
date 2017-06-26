#include "QueueScene.h"
#include "GameScene.h"
#include "PlayerBird.h"
#include "CoordinateUpdate.h"
#include "Network.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

QueueScene::QueueScene() {
}

QueueScene::~QueueScene() {
 //   CC_SAFE_RELEASE_NULL(player);
}

Scene* QueueScene::createScene() {
    QueueScene* layer = QueueScene::create();
    Scene* scene = Scene::createWithPhysics();

    scene->addChild(layer);

    return scene;
}

bool QueueScene::init() {
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite* queueTitle = Sprite::create("queueing.png");
    queueTitle->setAnchorPoint(Vec2(0.5, 0.5));
    queueTitle->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height
            - (queueTitle->getContentSize().height) * SCALE_FACTOR));
    queueTitle->getTexture()->setAliasTexParameters();

    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("bird_blue.plist");

    this->generateWorld();
    this->addBird();
    this->addChild(queueTitle, 1);
    this->schedule(schedule_selector(QueueScene::updateQueue));

    return true;
}

void QueueScene::generateWorld() {
    this->bottomLayer = Layer::create();
    this->middleLayer = Layer::create();
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
    this->addChild(this->bottomLayer, 1);
    this->addChild(this->middleLayer, 1);
    this->schedule(schedule_selector(QueueScene::updateQueue));
}
void QueueScene::startGameScene() {
    CCLOG("STARTING GAME");
    Director* director = Director::getInstance();
    Scene* scene = GameScene::createScene();
    director->replaceScene(scene);
}

void QueueScene::updateQueue(float dt) {
    this->time -= dt;
    if (this->time <= 0 || this->players.size() == PLAYER_QUEUE_MAX) {
        this->time = 0;
        this->startGameScene();
    }
}

void QueueScene::addBird() {
    PlayerBird* bird = PlayerBird::create();
    bird->animateFlappingLoop();
    bird->x_velocity = 0.0f;
    bird->getPhysicsBody()->setDynamic(false);
    if (players.empty()) {
        bird->setPosition(this->getStartingLocation());
    } else {
        PlayerBird* lastBird = players.back();
        bird->setPosition(lastBird->getPositionX()
                          - (lastBird->getContentSize().width * SCALE_FACTOR),
            lastBird->getPositionY());
    }
    this->players.push_back(bird);
    this->middleLayer->addChild(bird, Z_MIDDLE_LAYER);
}

Point QueueScene::getStartingLocation() {
    Size windowSize = Director::getInstance()->getVisibleSize();
    Point start = Vec2(windowSize.width / 2, windowSize.height / 2);
    return start;
}
