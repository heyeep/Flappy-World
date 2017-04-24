#ifndef SinglePlayerScene_h
#define SinglePlayerScene_h

#include <stdio.h>
#include "cocos2d.h"

class FlappyGame : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(FlappyGame);
};

#endif /* SinglePlayerScene_h */
