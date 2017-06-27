#ifndef LeaderboardScene_h
#define LeaderboardScene_h

#include "Network.h"
#include "cocos2d.h"
#include <stdio.h>

using namespace nlohmann;
USING_NS_CC;

class LeaderboardScene : public cocos2d::Layer {
public:
    /**
     *  \brief
     *
     *  \return
     */
    static cocos2d::Scene* createScene();

    /**
     *  \brief Method 'init' in cocos2d-x returns bool, instead of returning
     *   'id' in cocos2d-iphone. Creates the leaderboard.
     *
     *  \return bool
     */
    virtual bool init();

    /**
     *  \brief Implement the "static create()" method manually.
     *
     *  \return void
     */
    CREATE_FUNC(LeaderboardScene);

    /**
     *  \brief Constructor
     *
     *  \return LeaderboardScene
     */
    LeaderboardScene();

    /**
     *  \brief Destructor
     *
     *  \return ~LeaderboardScene
     */
    virtual ~LeaderboardScene();

private:
    /*!< Current size of the graphical window. */
    cocos2d::Size windowSize;

    /**
     *  \brief Connects to the scoreboard, grabs the data in json format, and
     *   then display the results. If a connection is not made, return to the
     *   previous screen.
     *
     *  \return void
     */
    void connectToScoreboard();

    /**
     *  \brief Displays names and scores of players on server.
     *
     *  \return void
     */
    void displayScores(nlohmann::json response);

    /**
     *  \brief Goes back to the main menu.
     *
     *  \return void
     */
    void gotoMainMenu();
};

#endif /* LeaderboardScene_h */
