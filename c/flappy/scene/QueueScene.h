#ifndef QueueScene_h
#define QueueScene_h

#include "SimpleAudioEngine.h"
#include "PlayerBird.h"
#include "cocos2d.h"
#include <stdio.h>

class QueueScene : public cocos2d::Layer {
public:
    /**
     *  \brief Creates a queue scene of birds flapping away.
     *
     *  \return QueueScene
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
     *  \brief Implement the "static create()" method manually.
     *
     *  \return void
     */
    CREATE_FUNC(QueueScene);

    /**
     *  \brief Constructor
     *
     *  \return QueueScene
     */
    QueueScene();

    /**
     *  \brief Destructor
     *
     *  \return QueueScene
     */
    virtual ~QueueScene();

private:
    /*!< The amount of time the queue is open before the game starts. */
    float time = DEFAULT_QUEUE_TIME;

    /*!< List of players in the queue. */
    std::vector<PlayerBird*> players;

    /*!< Reference that holds the current audio in the scene. */
    CocosDenshion::SimpleAudioEngine* sceneAudio;

    /*!< Default background. */
    cocos2d::Sprite* backgroundImg;

    /*!< Current size of the graphical window. */
    cocos2d::Size windowSize;

    /*!< Layer for the background. */
    cocos2d::Layer* bottomLayer;

    /*!< Layer for player. */
    cocos2d::Layer* middleLayer;

    /*!< The middlemost point on the screen at the start of the game. */
    static cocos2d::Point getStartingLocation();

    /**
     *  \brief Generates all layers and schedules the updateScene loop.
     *
     *  \return void
     */
    void generateWorld();

    /**
     *  \brief Adds a new bird to the end of the queue.
     *
     *  \return void
     */
    void addBird();

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

    /**
     *  \brief Starts the GameScene once the time is reaches zero or the player
     *  queue is full.
     *
     *  \return void
     */
    virtual void startGameScene();

    /**
     *  \brief Updates the time of the queue.
     *
     *  \return void
     */
    virtual void updateQueue(float dt);
};

#endif // QueueScene_h //
