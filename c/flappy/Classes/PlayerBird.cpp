#include "PlayerBird.h"

USING_NS_CC;

PlayerBird::PlayerBird()
{
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
