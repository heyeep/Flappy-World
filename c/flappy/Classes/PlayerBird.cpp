#include "PlayerBird.h"

USING_NS_CC;

PlayerBird::PlayerBird()
{
    this->speed = BIRD_SPEED;
}

PlayerBird::~PlayerBird()
{
}

/*
  Sets up the PlayerBird with an image, sets the anchor point to the bottom left (0, 0), and scale the sprite.
  It also sets the players PhysicsBody', which enables physics for the player.
 */
PlayerBird* PlayerBird::create()
{
    PlayerBird* player = new PlayerBird();

    // Returns: {Boolean} true if the sprite is initialized properly, false otherwise.
    if (player->initWithFile("bird_blue.png")) {
        cocos2d::PhysicsBody* body = PlayerBird::createPhysicsBody(player);
        player->setPhysicsBody(body);
        player->setAnchorPoint(Point::ZERO);
        player->setScale(SCALE_FACTOR);
        return player;
    }
    return NULL;
}

/*
  Creates a body with physics enabled.
 */
cocos2d::PhysicsBody* PlayerBird::createPhysicsBody(PlayerBird* player)
{
    cocos2d::PhysicsBody* pBody = PhysicsBody::createCircle(player->getContentSize().width/2,
                                                            PhysicsMaterial(0, 1, 0));
    return pBody;
}
/* 
   Called from FlappyGame::updatePlayer(dt). Get's the player's position and moves them right.
 */
void PlayerBird::update(float dt)
{
    Size wSize = Director::getInstance()->getVisibleSize();
    this->setPositionX(this->getPositionX() + speed);
    if (this->getPositionY() < 1) {
        this->setPositionY(wSize.height);
    }
}
