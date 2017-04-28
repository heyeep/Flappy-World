#include "PlayerBird.h"

USING_NS_CC;

PlayerBird::PlayerBird()
{
    speed = BIRD_SPEED;
}

PlayerBird::~PlayerBird()
{
}

PlayerBird* PlayerBird::create()
{
    PlayerBird* player = new PlayerBird();
    if (player->initWithFile("bird_blue.png")) {
        player->setAnchorPoint(Point::ZERO);
        player->setScale(SCALE_FACTOR);
        return player;
    }
    return NULL;
}

void PlayerBird::update(float dt)
{
    setPositionX(getPositionX() + speed);
}
