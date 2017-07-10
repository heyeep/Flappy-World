#include "Coin.h"

USING_NS_CC;

const std::vector<int> COIN_VALUES = {1, 5, 10};
const std::vector<std::string> COIN_IMAGES
    = { "coin_bronze.png", "coin_silver.png", "coin_gold.png"};

Coin::Coin() {
}

Coin::~Coin() {
}

Coin* Coin::create(const float xPos, const float yPos) {
    Coin* coin = new Coin();
    int randIndex = rand() % COIN_IMAGES.size();
    std::string coinImageName = COIN_IMAGES[randIndex];
    if (coin->initWithFile(coinImageName)) {
        coin->value = COIN_VALUES[randIndex];
        coin->xPos = xPos;
        coin->yPos = yPos;
        return coin;
    }
    return NULL;
}

void Coin::initCoin() {
    cocos2d::PhysicsBody* body = PhysicsBody::createCircle(
       this->getContentSize().width / 2, PhysicsMaterial(0.0f, 0.0f, 0.0f));
    body->setGravityEnable(false);
    body->setDynamic(false);
    body->setTag(TAG_COIN);
    body->setCategoryBitmask(BITMASK_CATEGORY_DEFAULT);
    body->setCollisionBitmask(BITMASK_COLLISION_DEFAULT);
    body->setContactTestBitmask(BITMASK_CONTACT_DEFAULT);
    this->setPhysicsBody(body);
    this->setScale(SCALE_FACTOR / 3);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setPosition(Vec2(xPos, yPos));
    this->getTexture()->setAliasTexParameters();
}

int Coin::getValue() {
    return value;;
}
