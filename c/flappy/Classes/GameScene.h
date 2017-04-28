#ifndef SinglePlayerScene_h
#define SinglePlayerScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "Constants.h"
#include "PlayerBird.h"


class FlappyGame : public cocos2d::Layer {
public:
    PlayerBird* player;
   
    cocos2d::Size windowSize;
    cocos2d::Layer* bottomLayer;
    cocos2d::Layer* middleLayer;
    cocos2d::Layer* topLayer;
    cocos2d::Layer* hudLayer;
    cocos2d::ParallaxNode* parallaxNode;
    cocos2d::Sprite* backgroundImg;
    cocos2d::Follow* cameraTarget;

    static cocos2d::Scene* createScene();
    static cocos2d::Point getStartingLocation();
    
    virtual bool init();

    void generateWorld();
    void generateBottomLayer();
    void generateMiddleLayer();

    void addPlayer();
    void setCameraTarget();

    void updateScene(float dt);
    void updatePlayer(float dt);

    CREATE_FUNC(FlappyGame);
};

#endif /* SinglePlayerScene_h */
