#ifndef SinglePlayerScene_h
#define SinglePlayerScene_h

#include <stdio.h>
#include "cocos2d.h"

class SinglePlayerGame : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(SinglePlayerGame);
};

#endif /* SinglePlayerScene_h */
