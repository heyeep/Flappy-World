#include "PlayerBird.h"

USING_NS_CC;

PlayerBird::PlayerBird() {
    this->speed = BIRD_SPEED;
}

PlayerBird::~PlayerBird() {
}

/*
  Sets up the PlayerBird with an image, sets the anchor point to the bottom left
  (0, 0), and scale the sprite. It also sets the players PhysicsBody', which
  enables physics for the player.
 */
PlayerBird* PlayerBird::create() {
    PlayerBird* player = new PlayerBird();

    // Assigns the player with a sprite and a physics body. Loads it from cache.
    if (player->initWithSpriteFrameName("bird_blue_01.png")) {
        cocos2d::PhysicsBody* body = PlayerBird::createPhysicsBody(player);
        player->frames = PlayerBird::getFrames();
        player->setPhysicsBody(body);
        player->getTexture()->setAliasTexParameters();
        player->setAnchorPoint(Vec2(0.5, 0.5));
        player->setScale(SCALE_FACTOR);
        player->flapDegrees
            = -((BIRD_SPEED / (player->getContentSize().width / 2)) * 50);
        return player;
    }
    return NULL;
}

/*
  Grabs the frames from the bird image in the cache.
 */
cocos2d::Vector<SpriteFrame*> PlayerBird::getFrames() {
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*> newFrames;
    char str[100] = { 0 };
    for (int i = 1; i < numberOfFrames; i++) {
        sprintf(str, "bird_blue_%02d.png", i);
        newFrames.pushBack(cache->getSpriteFrameByName(str));
    }
    return newFrames;
}

/*
  Creates a body with physics enabled.
 */
cocos2d::PhysicsBody* PlayerBird::createPhysicsBody(PlayerBird* player) {
    cocos2d::PhysicsBody* pBody = PhysicsBody::createCircle(
        player->getContentSize().width / 2, PhysicsMaterial(0, 1, 0));
    return pBody;
}

/*
   Called from GameScene::updatePlayer(dt). Calls any update the player needs.
 */
void PlayerBird::update(float dt) {
    this->updatePosition();
    this->updateAngle(dt);
}

/*
  Moves the player constantly right.
 */
void PlayerBird::updatePosition() {
    float y_velocity = this->getPhysicsBody()->getVelocity().y;
    this->getPhysicsBody()->setVelocity(Vec2(300, y_velocity));
}

/*
  Rotates the player's sprite based on velocity. Multiplying it by delta
  should give it a smoother transition
 */
void PlayerBird::updateAngle(float dt) {
    Vec2 vec = this->getPhysicsBody()->getVelocity() * dt;
    float degrees = CC_RADIANS_TO_DEGREES(-vec.getAngle());
    this->setRotation(degrees);
}

/*
  Moves and rotates the player upwards, runs the flap animation.
 */
void PlayerBird::flap() {
    Animation* anim = new Animation;
    anim->initWithSpriteFrames(frames, 0.05f);
    this->runAction(Animate::create(anim));
    this->setRotation(flapDegrees);
    this->getPhysicsBody()->setVelocity(BIRD_VELOCITY);
}

/*
   Returns true if the player's y location is below 0 or if it's above the
   height of the window.
 */
bool PlayerBird::isDead(cocos2d::Size windowSize) {
    return (this->getPositionY() <= 0
        || (this->getPositionY()
                   + (this->getContentSize().height * SCALE_FACTOR)
               >= windowSize.height));
}
