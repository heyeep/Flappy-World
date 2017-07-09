#include "Points.h"

USING_NS_CC;

const float POINT_W = 10.0f;
const float POINT_H = 250.0f;

Points* Points::create(const int value, const float xPos) {
    Points* points = new Points();
    points->value = value;
    points->xPos = xPos;

    return points;
}

void Points::initPoints() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::PhysicsBody* body = PhysicsBody::createBox(
        Size(POINT_W, POINT_H), PhysicsMaterial(0.0f, 0.0f, 0.0f));
    body->setGravityEnable(false);
    body->setDynamic(false);
    body->setTag(TAG_POINTS);
    body->setCategoryBitmask(BITMASK_CATEGORY_DEFAULT);
    body->setCollisionBitmask(BITMASK_COLLISION_DEFAULT);
    body->setContactTestBitmask(BITMASK_CONTACT_DEFAULT);
    this->setPhysicsBody(body);
    this->setScaleY(SCALE_FACTOR);
    this->setAnchorPoint(Point::ZERO);
    this->setPosition(xPos, 0);
    this->setTextureRect(Rect(0, 0, 1, visibleSize.height));
    this->setColor(Color3B(255, 255, 255));
}
