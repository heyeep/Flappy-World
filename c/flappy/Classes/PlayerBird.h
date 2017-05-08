#ifndef PlayerBird_h
#define PlayerBird_h

#include "Constants.h"
#include "cocos2d.h"
#include <stdio.h>

const int numberOfFrames = 3;

class PlayerBird : public cocos2d::Sprite {
public:
    int speed;
    cocos2d::Animation* flapAnimation;
    cocos2d::Vector<cocos2d::SpriteFrame*> frames;

    static PlayerBird* create();
    static cocos2d::PhysicsBody* createPhysicsBody(PlayerBird* player);
    static cocos2d::Vector<cocos2d::SpriteFrame*> getFrames();

    void update(float dt);
    void updatePosition();
    void updateAngle(float dt);
    void flap();
    void flapAnimate();
    void setFlapAnimation();

    bool isDead(cocos2d::Size windowSize);

    PlayerBird();
    virtual ~PlayerBird();

private:
    float flapDegrees;
};

#endif /* PlayerBird_h */
