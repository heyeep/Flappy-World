#ifndef SinglePlayerScene_h
#define SinglePlayerScene_h

#include <stdio.h>
#include "cocos2d.h"

#define SCALE_FACTOR 2.5
#define Z_BOTTOM_LAYER 0
#define Z_MIDDLE_LAYER 1
#define Z_TOP_LAYER 2
#define Z_HUD_LAYER 3

class FlappyGame : public cocos2d::Layer {
public:
    cocos2d::Size windowSize;
    /*
    _bottomLayer = background
    _middleLayer = pipes/obstacles
    _topLayer = objects
    _hudLayer = hud/gui/etc
     */
    cocos2d::Layer* _bottomLayer;
    cocos2d::Layer* _middleLayer;
    cocos2d::Layer* _topLayer;
    cocos2d::Layer* _hudLayer;

    cocos2d::ParallaxNode* _parallaxNode;
    cocos2d::Sprite* backgroundImg;
    
    static cocos2d::Scene* createScene();
    virtual bool init();

    void generateWorld();
    void generateBottomLayer();
    void generateMiddleLayer();

    CREATE_FUNC(FlappyGame);
};

#endif /* SinglePlayerScene_h */
