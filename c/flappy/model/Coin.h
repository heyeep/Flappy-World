#ifndef Coin_h
#define Coin_h

#include "Constants.h"
#include "cocos2d.h"
#include <stdio.h>

class Coin : public cocos2d::Sprite {
public:
    /**
     * \brief Sets up the coin.
     *
     * \return Coin*
     */
    static Coin* create();

    /**
     * \brief Sets up the coin with a value and xPos.
     *
     * \return Coins*
     */
    static Coin* create(const float xPos, const float yPos);

    /**
     * \brief Initializes the point object.
     *
     * \return void
     */
    void initCoin();

    /** \brief Gets the value of the coin.
     *
     *
     * \ return int;
     */
    int getValue();

    /**
     *  \brief Constructor
     *
     *  \return Coin
     */
    Coin();

    /**
     *  \brief Destructor
     *
     *  \return ~Coin
     */
    virtual ~Coin();

private:
    /*!< How much each coin object is worth. */
    int value;

    /*!< X coordinate position. */
    float xPos;

    /*!< Y coordinate position. */
    float yPos;
};

#endif /* Coin_h */
