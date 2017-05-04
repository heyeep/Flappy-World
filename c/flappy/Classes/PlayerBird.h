#ifndef PlayerBird_h
#define PlayerBird_h

#include "Constants.h"
#include "cocos2d.h"
#include <stdio.h>

class PlayerBird : public cocos2d::Sprite {
public:
    int speed;

    static PlayerBird* create();
    static cocos2d::PhysicsBody* createPhysicsBody(PlayerBird* player);

    void update(float dt);
    void flap();

    bool isDead(cocos2d::Size windowSize);

    PlayerBird();
    virtual ~PlayerBird();
};

#endif /* PlayerBird_h */
