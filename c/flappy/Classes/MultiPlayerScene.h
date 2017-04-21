#ifndef MultiPlayerScene_h
#define MultiPlayerScene_h

#include <stdio.h>
#include "cocos2d.h"

class MultiPlayerGame : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    CREATE_FUNC(MultiPlayerGame);
};

#endif /* MultiPlayerScene_h */
