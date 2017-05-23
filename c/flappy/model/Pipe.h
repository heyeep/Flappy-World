#ifndef Pipe_h
#define Pipe_h

#include "Constants.h"
#include "cocos2d.h"
#include <stdio.h>

class Pipe : public cocos2d::Node {
public:
    static Pipe* create();
    static Pipe* create(
        const std::string pipeType, const float xPos, const float yPos);

    void initPipe();
    void update(float dt);

private:
    std::string pipeType;
    float xPos;
    float yPos;
    void initTopPipe(float xPos, float yPos);
    void initBottomPipe(const float xPos, const float yPos);
};

#endif /* Pipe_h */
