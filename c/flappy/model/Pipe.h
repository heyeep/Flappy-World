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
    std::string getPipeType() const;
    void setPipeType(std::string pipeType);
    float getXPos() const;
    void setXPos(float xPos);
    float getYPos() const;
    void setYPos(float yPos);

private:
    std::string pipeType;
    float xPos;
    float yPos;
    void initTopPipe(float xPos, float yPos);
    void initBottomPipe(const float xPos, const float yPos);
};

#endif /* Pipe_h */
