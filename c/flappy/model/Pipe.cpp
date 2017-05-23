#include "Pipe.h"

USING_NS_CC;

const std::string PIPE_TYPE_TOP = "top";
const std::string PIPE_TYPE_BOTTOM = "bottom";
const std::string PIPE_GREEN_BASE_IMAGE = "pipe_green_base.png";
const std::string PIPE_GREEN_BOTTOM_IMAGE = "pipe_green_bottom.png";
const std::string PIPE_GREEN_TOP_IMAGE = "pipe_green_top.png";

const float TOP_H = 13.0f;
const float BASE_H = 26.0f;
const float BASE_SCALE_Y = 10.0f;

const float PIPE_W = 26.0f;
const float PIPE_H = TOP_H + BASE_H * BASE_SCALE_Y;

Pipe* Pipe::create(
    const std::string pipeType, const float xPos, const float yPos) {
    Pipe* pipe = new Pipe();
    pipe->pipeType = pipeType;
    pipe->xPos = xPos;
    pipe->yPos = yPos;

    return pipe;
}

void Pipe::initPipe() {
    if (pipeType == PIPE_TYPE_TOP) {
        initTopPipe(xPos, yPos);
    } else if (pipeType == PIPE_TYPE_BOTTOM) {
        initBottomPipe(xPos, yPos);
    } else {
        // default
        initTopPipe(xPos, yPos);
    }
}

void Pipe::initTopPipe(const float xPos, const float yPos) {

    cocos2d::PhysicsBody* body = PhysicsBody::createBox(
        cocos2d::Size(PIPE_W, PIPE_H), PhysicsMaterial(0, 0, 0));
    body->setGravityEnable(false);
    body->setDynamic(false);
    body->setPositionOffset(cocos2d::Vec2(
        ((PIPE_W * SCALE_FACTOR) / 2.0f), ((PIPE_H * SCALE_FACTOR) / 2.0f)));
    this->setPhysicsBody(body);
    this->setScale(SCALE_FACTOR);
    this->setAnchorPoint(Point::ZERO);
    this->setPosition(xPos, yPos);

    cocos2d::Sprite* top = new cocos2d::Sprite();
    top->initWithFile(PIPE_GREEN_TOP_IMAGE);
    top->setAnchorPoint(Point::ZERO);
    top->setPosition(Point::ZERO);

    cocos2d::Sprite* base = new cocos2d::Sprite();
    base->initWithFile(PIPE_GREEN_BASE_IMAGE);
    base->setAnchorPoint(Point::ZERO);
    base->setPosition(0.0f, top->getContentSize().height);
    base->setScaleY(BASE_SCALE_Y);

    this->addChild(top);
    this->addChild(base);
}

void Pipe::initBottomPipe(const float xPos, const float yPos) {
    cocos2d::PhysicsBody* body = PhysicsBody::createBox(
        cocos2d::Size(PIPE_W, PIPE_H), PhysicsMaterial(0, 0, 0));
    body->setGravityEnable(false);
    body->setDynamic(false);
    body->setPositionOffset(cocos2d::Vec2(((PIPE_W * SCALE_FACTOR) / 2.0f),
        -1.0f * ((PIPE_H * SCALE_FACTOR) / 2.0f)));
    this->setPhysicsBody(body);
    this->setScale(SCALE_FACTOR);
    this->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
    this->setPosition(xPos, yPos);

    cocos2d::Sprite* bottom = new cocos2d::Sprite();
    bottom->initWithFile(PIPE_GREEN_BOTTOM_IMAGE);
    bottom->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
    bottom->setPosition(Point::ZERO);

    cocos2d::Sprite* base = new cocos2d::Sprite();
    base->initWithFile(PIPE_GREEN_BASE_IMAGE);
    base->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
    base->setPosition(0.0f, -1.0f * bottom->getContentSize().height);
    base->setScaleY(BASE_SCALE_Y);

    this->addChild(bottom);
    this->addChild(base);
}

void Pipe::update(float dt) {
}
