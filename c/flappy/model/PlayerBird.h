#ifndef PlayerBird_h
#define PlayerBird_h

#include "Constants.h"
#include "cocos2d.h"
#include <stdio.h>

USING_NS_CC;

/*!< Maximum number of frames the player sprite has. */
const int NUMBER_OF_FRAMES = 3;

class PlayerBird : public cocos2d::Sprite {
public:
    /*!< Maximum number of frames the player sprite has. */
    cocos2d::Animation* flapAnimation;

    /*!< Maximum number of frames the player sprite has. */
    cocos2d::Vector<cocos2d::SpriteFrame*> frames;

    /**
     *  \brief  Sets up the PlayerBird with an image, sets the anchor point to
     *   the bottom left (0, 0), and scale the sprite. It also sets the players
     *   PhysicsBody', which enables physics for the player.
     *
     *  \return PlayerBird
     */
    static PlayerBird* create();

    /**
     *  \brief Returns a PhysicsBody for the player.
     *
     *  \return PhysicsBody
     */
    static cocos2d::PhysicsBody* createPhysicsBody(PlayerBird* player);

    /**
     *  \brief Grabs the player's frames from the cache.
     *
     *  \return SpriteFrame
     */
    static cocos2d::Vector<cocos2d::SpriteFrame*> getFrames();

    /**
     *  \brief Updates the player's position, status, angle, etc.
     *
     *  \return void
     */
    void update(float dt);

    /**
     *  \brief Constantly moves the player right.
     *
     *  \return void
     */
    void updatePosition();

    /**
     *  \brief Rotates the player downwards, matching velocity.
     *
     *  \return void
     */
    void updateAngle(float dt);

    /**
     *  \brief Changes the velocity, rotation and animates the player to
     *   replicate "flapping".
     *
     *  \return void
     */
    void flap();

    /**
     *  \brief Plays the flapping animation once.
     *
     *  \return void
     */
    void flapAnimate();

    /**
     *  \brief Initiates the player's flapping animations.
     *
     *  \return void
     */
    void setFlapAnimation();

    /**
     *  \brief Return true if the player meet's requirements for death. For
     *   instance, flying off screen.
     *
     *  \return bool status
     */
    bool isDead(cocos2d::Size windowSize);

    /**
     *  \brief Constructor
     *
     *  \return PlayerBird
     */
    PlayerBird();

    /**
     *  \brief Destructor
     *
     *  \return ~PlayerBird
     */
    virtual ~PlayerBird();

private:
    /*!< Current speed the bird is going. */
    int speed;

    /*!< THe amount of degrees the bird must rotation to match animation. */
    float flapDegrees;
};

#endif /* PlayerBird_h */
