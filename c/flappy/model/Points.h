#ifndef Points_h
#define Points_h

#include "Constants.h"
#include "cocos2d.h"
#include <stdio.h>

class Points : public cocos2d::Sprite {
public:
    /**
     * \brief Sets up the point.
     *
     * \return Points*
     */
    static Points* create();

    /**
     * \brief Sets up the point with a value and xPos.
     *
     * \return Points*
     */
    static Points* create(const int value, const float xPos);

    /**
     * \brief Initializes the point object.
     *
     * \return void
     */
    void initPoints();

    int getValue();
private:
    /*!< How much each point object is worth. */
    int value;

    /*!< X coordinate position. */
    float xPos;
};

#endif /* Points_h */
