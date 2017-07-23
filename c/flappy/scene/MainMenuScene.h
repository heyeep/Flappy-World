#ifndef MainMenuScene_h
#define MainMenuScene_h

#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include <stdio.h>

class MainMenu : public cocos2d::Layer {
public:
    /**
     *  \brief No identifier in C++, it's recommended returning the class
     *   instance point.
     *
     *  \return MainMenu
     */
    static cocos2d::Scene* createScene();

    /**
     *  \brief Method 'init' in cocos2d-x returns bool, instead of returning
     *   'id' in cocos2d-iphone. Creates the splash screen.
     *
     *  \return bool
     */
    virtual bool init();

    /**
     *  \brief Callback to start GameScene.
     *
     *  \return void
     */
    void startGameCall(cocos2d::Ref* pSender);

    /**
     *  \brief Callback to start LeaderboardScene.
     *
     *  \return void
     */
    void switchToLeaderboards(cocos2d::Ref* refPointer);

    /**
     *  \brief Implement the "static create()" method manually.
     *
     *  \return void
     */
    CREATE_FUNC(MainMenu);

    /**
     *  \brief Destructor
     *
     *  \return ~MainMenu
     */
    virtual ~MainMenu();

private:
    /*!< Reference that holds the current audio in the scene. */
    CocosDenshion::SimpleAudioEngine* sceneAudio;

    /**
     *  \brief Initializes the audio.
     *
     *  \return void
     */
    void initAudio();

    /**
     *  \brief Preload all audio pertaining to the scene.
     *
     *  \return void
     */
    void preloadAudio();

    /**
     *  \brief Plays the background music.
     *
     *  \return void
     */
    void playBackgroundMusic();

    /**
     *  \brief Stops the current running background music.
     *
     *  \return void
     */
    void stopBackgroundMusic();
};

#endif // MainMenuScene_h //
