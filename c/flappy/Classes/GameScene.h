#ifndef SinglePlayerScene_h
#define SinglePlayerScene_h

#include <stdio.h>
#include "cocos2d.h"
#include "Constants.h"
#include "PlayerBird.h"

class GameScene : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();

    CREATE_FUNC(GameScene);

private:
    PlayerBird* player;
    
    cocos2d::Size windowSize;
    cocos2d::PhysicsWorld* sceneWorld;
    cocos2d::Layer* bottomLayer;
    cocos2d::Layer* middleLayer;
    cocos2d::Layer* topLayer;
    cocos2d::Layer* hudLayer;
    cocos2d::ParallaxNode* parallaxNode;
    cocos2d::Sprite* backgroundImg;
    cocos2d::Sprite* cameraObject;
    cocos2d::Follow* cameraTarget;
    cocos2d::EventListenerMouse* mouseListener;
    static cocos2d::Point getStartingLocation();

    void generateWorld();
    void generateBottomLayer();
    void generateMiddleLayer();

    void addPlayer();
    void addCameraObject();
    void setCameraTarget(cocos2d::Sprite* follow);

    void updateScene(float dt);
    void updatePlayer(float dt);

    void setMouseListeners();
    void onMouseDown(cocos2d::Event* event);
};

#endif /* GameScene_h */
