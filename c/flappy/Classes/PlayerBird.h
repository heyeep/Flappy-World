#ifndef PlayerBird_h
#define PlayerBird_h

#include <stdio.h>
#include "cocos2d.h"
#include "Constants.h"

class PlayerBird : public cocos2d::Sprite
{
public:
    static PlayerBird* create();
    
    PlayerBird();
    virtual ~PlayerBird();
};

#endif /* PlayerBird_h */
