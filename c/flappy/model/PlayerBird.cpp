#include "PlayerBird.h"
#include "GameScene.h"

PlayerBird::PlayerBird() {
    this->speed = BIRD_SPEED;
}

PlayerBird::~PlayerBird() {
    CC_SAFE_RELEASE_NULL(flapAnimation);
}

PlayerBird* PlayerBird::create() {
    PlayerBird* player = new PlayerBird();

    // Assigns the player with a sprite and a physics body. Loads it from cache.
    if (player->initWithSpriteFrameName("bird_blue_01.png")) {
        cocos2d::PhysicsBody* body = PlayerBird::createPhysicsBody(player);
        player->frames = PlayerBird::getFrames();
        player->initFlapAnimation();
        player->setPhysicsBody(body);
        player->getTexture()->setAliasTexParameters();
        player->setAnchorPoint(Vec2(0.5, 0.5));
        player->setScale(SCALE_FACTOR);
        player->initCollision();

        // Using the bird's speed and radius to determine how much the bird
        // should turn seemed the most reasonable. I went from 0-100 as the
        // multiplier and 50 seems to be the middle ground.
        player->flapDegrees
            = -((BIRD_SPEED / (player->getContentSize().width / 2)) * 50);
        return player;
    }
    return NULL;
}

int PlayerBird::getServerId() {
    return this->serverId;
}

cocos2d::Vector<SpriteFrame*> PlayerBird::getFrames() {
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> newFrames;
    char str[20] = { 0 };
    for (int i = 1; i < NUMBER_OF_FRAMES; i++) {
        sprintf(str, "bird_blue_%02d.png", i);
        newFrames.pushBack(cache->getSpriteFrameByName(str));
    }
    return newFrames;
}

cocos2d::PhysicsBody* PlayerBird::createPhysicsBody(PlayerBird* player) {
    cocos2d::PhysicsBody* pBody = PhysicsBody::createCircle(
        player->getContentSize().width / 2, PhysicsMaterial(0, 1, 0));
    return pBody;
}

void PlayerBird::update(float dt) {
    if (!this->dead) {
        this->updatePosition();
        this->updateAngle(dt);
    }
}

void PlayerBird::updatePosition() {
    float y_velocity = this->getPhysicsBody()->getVelocity().y;
    this->getPhysicsBody()->setVelocity(Vec2(x_velocity, y_velocity));
}

void PlayerBird::updateAngle(float dt) {
    Vec2 vec = this->getPhysicsBody()->getVelocity() * dt;
    float degrees = CC_RADIANS_TO_DEGREES(-vec.getAngle());
    this->setRotation(degrees);
}

void PlayerBird::flap() {
    this->getPhysicsBody()->setVelocity(BIRD_VELOCITY);
    this->setRotation(flapDegrees);
    this->animateFlapping();
}

void PlayerBird::animateFlapping() {
    this->runAction(Animate::create(this->flapAnimation));
}

void PlayerBird::animateFlappingLoop() {
    this->flapAnimation->setLoops(-1);
    this->runAction(Animate::create(this->flapAnimation));
}
void PlayerBird::initFlapAnimation() {
    this->flapAnimation = Animation::createWithSpriteFrames(frames, 0.05f);
    this->flapAnimation->retain();
}

bool PlayerBird::isDead(cocos2d::Size windowSize) {
    return (this->getPositionY() <= 0
        || (this->getPositionY()
                   + (this->getContentSize().height * SCALE_FACTOR)
               >= windowSize.height));
}

void PlayerBird::initCollision() {
    this->getPhysicsBody()->setTag(TAG_PLAYER);
    this->getPhysicsBody()->setCategoryBitmask(BITMASK_CATEGORY_DEFAULT);
    this->getPhysicsBody()->setCollisionBitmask(BITMASK_COLLISION_DEFAULT);
    this->getPhysicsBody()->setContactTestBitmask(BITMASK_CONTACT_DEFAULT);
}
