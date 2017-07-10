#include "PlayerBird.h"

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
    this->updatePosition();
    this->updateAngle(dt);
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
    EventListenerPhysicsContact* contactListener
        = EventListenerPhysicsContact::create();
    contactListener->onContactBegin
        = CC_CALLBACK_1(PlayerBird::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        contactListener, this);
}

bool PlayerBird::onContactBegin(cocos2d::PhysicsContact& contact) {
    PhysicsBody* bodyA = contact.getShapeA()->getBody();
    PhysicsBody* bodyB = contact.getShapeB()->getBody();
    if (bodyA && bodyB) {
        // Collision for dense objects
        if ((bodyA->getCategoryBitmask() & bodyB->getCollisionBitmask())
                == BITMASK_COLLISION_DENSE
            || (bodyB->getCategoryBitmask() & bodyA->getCollisionBitmask())
                == BITMASK_COLLISION_DENSE) {
            // Player <--> Pipes
            if ((bodyA->getTag() == TAG_PLAYER && bodyB->getTag() == TAG_PIPE)
                || (bodyA->getTag() == TAG_PIPE
                       && bodyB->getTag() == TAG_PLAYER)) {
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
                if (bodyA->getTag() == TAG_POINTS) pointN = bodyA->getNode();
                if (bodyB->getTag() == TAG_POINTS) pointN = bodyB->getNode();
                Points* pointP = dynamic_cast<Points*>(pointN);
                if (pointP) {
                    this->points += pointP->getValue();
                }
            // Player <--> Coin
            } else if ((bodyA->getTag() == TAG_PLAYER
                           && bodyB->getTag() == TAG_COIN)
                || (bodyA->getTag() == TAG_COIN
                       && bodyB->getTag() == TAG_PLAYER)) {
                CCLOG("Collision Detected: Coins");
                Node* coinN;
                if (bodyA->getTag() == TAG_COIN) coinN = bodyA->getNode();
                if (bodyB->getTag() == TAG_COIN) coinN = bodyB->getNode();
                Coin* coinP = dynamic_cast<Coin*>(coinN);
                if (coinP) {
                    this->coins += coinP->getValue();
                    coinP->removeFromParent();
                }
            }
        }
    }

    return true;
}

void PlayerBird::death() {
    Director* director = Director::getInstance();
    Scene* scene = MainMenu::createScene();

    director->replaceScene(scene);
}
