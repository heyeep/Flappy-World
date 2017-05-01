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

    // Assigns the player with a sprite and a physics body.
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
   Called from GameScene::updatePlayer(dt). Get's the player's position and moves them right.
   If the player's y location is below 0, set it to the top.
 */
void PlayerBird::update(float dt)
{
    Size windowSize = Director::getInstance()->getVisibleSize();
    this->setPositionX(this->getPositionX() + speed);
    if (!this->getPositionY()) {
        this->setPositionY(windowSize.height);
    }
}
