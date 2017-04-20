#ifndef MainMenuScene_h
#define MainMenuScene_h

#include <stdio.h>
#include "cocos2d.h"

class MainMenu : public cocos2d::Layer {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    void callSinglePlayer(cocos2d::Ref* pSender);
    void callMultiPlayer(cocos2d::Ref* pSender);
    
    CREATE_FUNC(MainMenu);
};

#endif // MainMenuScene_h //
