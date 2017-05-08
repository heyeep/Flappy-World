#include "PlayerBird.h"

PlayerBird::PlayerBird() {
    this->speed = BIRD_SPEED;
}

PlayerBird::~PlayerBird() {
    CC_SAFE_DELETE(flapAnimation);
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
        player->setFlapAnimation();
        player->setPhysicsBody(body);
        player->getTexture()->setAliasTexParameters();
        player->setAnchorPoint(Vec2(0.5, 0.5));
        player->setScale(SCALE_FACTOR);

        // Using the bird's speed and radius to determine how much the bird
        // should turn seemed the most reasonable. I went from 0-100 as the
        // multiplier and 50 seems to be the middle ground.
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
    char str[20] = { 0 };
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
    this->getPhysicsBody()->setVelocity(BIRD_VELOCITY);
    this->setRotation(flapDegrees);
    this->flapAnimate();
}

/*
  Runs the conversion from animation to animate, also make it graphically flap.
 */
void PlayerBird::flapAnimate() {
    this->runAction(Animate::create(this->flapAnimation));
}

/*
  Sets up player's flapping animations to be used via Animate.
 */
void PlayerBird::setFlapAnimation() {
    this->flapAnimation = Animation::createWithSpriteFrames(frames, 0.05f);
    this->flapAnimation->retain();
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
