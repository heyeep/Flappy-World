#ifndef GameScene_h
#define GameScene_h

#include "Constants.h"
#include "MainMenuScene.h"
#include "Pipe.h"
#include "PlayerBird.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include <list>
#include <stdio.h>

class GameScene : public cocos2d::Layer {
public:
    /**
     *  \brief Creates a scene with built-in physics.
     *
     *  \return cocos2d::Scene*
     */
    static cocos2d::Scene* createScene(std::list<Pipe*> pipes);
    static cocos2d::Scene* createScene();

    /**
     *  \brief This function is called from createScene() and generates the
     *   world.
     *
     *  \return bool
     */
    virtual bool init();

    virtual void loadStage(std::list<Pipe*> pipes);

    /**
     *  \brief Initiates all sprites adding them to the SpriteFrameCache.
     *
     *  \return void
     */
    virtual void initSprites();

    /**
     *  \brief Initializes the client as a touch listener.
     *
     *  \return void
     */
    virtual void initTouchListeners();

    /**
     *  \brief Returns true when the client touches the screen.
     *
     *  \return bool status
     */
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);

    /**
     *  \brief What happens when the client touches the screen.
     *
     *  \return void
     */
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);

    /**
     *  \brief Implement the "static create()" method manually.
     *
     *  \return void
     */
    CREATE_FUNC(GameScene);

    /**
     *  \brief Constructor
     *
     *  \return GameScene
     */
    GameScene();

    /**
     *  \brief Destructor
     *
     *  \return ~GameScene
     */
    virtual ~GameScene();

private:
    /*!< Client's sprite. */
    PlayerBird* player;

    /*!< Holds the list of players. */
    std::vector<PlayerBird*> players;

    /*!< Current size of the graphical window. */
    cocos2d::Size windowSize;

    /*!< Layer for the background. */
    cocos2d::Layer* bottomLayer;

    /*!< Layer for player. */
    cocos2d::Layer* middleLayer;

    /*!< Layer for objects, overlays, etc. */
    cocos2d::Layer* topLayer;

    /*!< Layer for the HUD. */
    cocos2d::Layer* hudLayer;

    /*!< Holds all the layers */
    cocos2d::ParallaxNode* parallaxNode;

    /*!< Default background. */
    cocos2d::Sprite* backgroundImg;

    /*!< An invisible sprite that shares the same x-coord as the client. */
    cocos2d::Sprite* cameraObject;

    /*!< Controls the client's camera. */
    cocos2d::Follow* cameraTarget;

    /*!< Client doing mouse commands. */
    cocos2d::EventListenerMouse* mouseListener;

    /*!< Client doing touch commands. */
    cocos2d::EventListenerTouchOneByOne* touchListener;

    /*!< Reference that holds the current audio in the scene. */
    CocosDenshion::SimpleAudioEngine* sceneAudio;

    /**
     *  \brief Retrieves the middlemost point the screen.
     *
     *  \return cocos2d::Point
     */
    static cocos2d::Point getStartingLocation();

    /**
     *  \brief Generates all layers and schedules the updateScene loop.
     *
     *  \return void
     */
    void generateWorld();

    /**
     *  \brief Generates the background layer.
     *
     *  \return void
     */
    void generateBottomLayer();

    /**
     *  \brief Generates the middle layer.
     *
     *  \return void
     */
    void generateMiddleLayer();

    /**
     *  \brief Initiates and adds the player sprite the game.
     *
     *  \return void
     */
    void addPlayer();

    /**
     *  \brief Initiates and adds an invisible object for the camera to follow.
     *
     *  \return void
     */
    void initCamera();

    /**
     *  \brief Sets the camera to follow the invisible camera object.
     *
     *  \return void
     */
    void setCameraTarget(cocos2d::Sprite* follow);

    /**
     *  \brief Updates everything that happens in the current sceen.
     *
     *  \return void
     */
    void updateScene(float dt);

    /**
     *  \brief Updates the player's sprite and state.
     *
     *  \return void
     */
    void updatePlayer(float dt);

    /**
     *  \brief Check's whether the player is in state where they can be
     * considered death.
     *
     *  \return void
     */
    void playerDeathCheck();

    /**
     *  \brief Restarts the game.
     *
     *  \return void
     */
    void death();

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
     *  \brief Initializes mouse listeners.
     *
     *  \return void
     */
    void initMouseListeners();

    /**
     *  \brief This is called when the player clicks using their mouse, making
     *   the bird "flap".
     *
     *  \return void
     */
    void onMouseDown(cocos2d::Event* event);

    /**
     *  \brief Initializes collision testing by creating an object for the
     *   player to collide into.
     *
     *  \return void
     */
    void initCollisionTest();
};

#endif /* GameScene_h */
