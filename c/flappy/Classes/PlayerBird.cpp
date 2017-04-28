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
  Sets up the PlayerBird with an image, sets the anchor point to the bottom left (0, 0), and scale the sprite
 */
PlayerBird* PlayerBird::create()
{
    PlayerBird* player = new PlayerBird();

    // Returns: {Boolean} true if the sprite is initialized properly, false otherwise.
    if (player->initWithFile("bird_blue.png")) {
        player->setAnchorPoint(Point::ZERO);
        player->setScale(SCALE_FACTOR);
        return player;
    }
    return NULL;
}

/* 
   Called from FlappyGame::updatePlayer(dt). Get's the player's position and moves them right.
 */
void PlayerBird::update(float dt)
{
    this->setPositionX(getPositionX() + speed);
}
