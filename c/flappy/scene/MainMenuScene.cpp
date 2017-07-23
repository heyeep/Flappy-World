#include "MainMenuScene.h"
#include "GameScene.h"
#include "LeaderboardScene.h"
#include "Network.h"
#include "QueueScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

MainMenu::~MainMenu() {
    Network::getInstance()->unsubscribe(JOIN_ROOM_KEY, this);
}

Scene* MainMenu::createScene() {
    Scene* scene = Scene::create();
    Layer* layer = MainMenu::create();

    scene->addChild(layer);

    return scene;
}

bool MainMenu::init() {
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite* gameTitle = Sprite::create("flappybird_title.png");
    gameTitle->setAnchorPoint(Vec2(0.5, 0.5));
    gameTitle->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height
            - (gameTitle->getContentSize().height) * SCALE_FACTOR));
    gameTitle->getTexture()->setAliasTexParameters();
    gameTitle->setScale(SCALE_FACTOR);

    // MenuItemSprite does not allow disabling of anti-aliasing. This is a work
    // around.
    Sprite* playButton = Sprite::create("play_button.png");
    Sprite* playButtonSel = Sprite::create("play_button-s.png");
    Sprite* leaderButton = Sprite::create("leaderboard.png");
    Sprite* leaderButtonSel = Sprite::create("leaderboard-s.png");
    playButton->getTexture()->setAliasTexParameters();
    playButtonSel->getTexture()->setAliasTexParameters();
    leaderButton->getTexture()->setAliasTexParameters();
    leaderButtonSel->getTexture()->setAliasTexParameters();

    MenuItemSprite* startButton = MenuItemSprite::create(playButton,
        playButtonSel,
        CC_CALLBACK_1(MainMenu::startGameCall, this));
    startButton->setPosition(
        Vec2(gameTitle->getPositionX(), visibleSize.height / 2));
    startButton->setScale(SCALE_FACTOR / 2);

    MenuItemSprite* leaderboardButton = MenuItemSprite::create(leaderButton,
        leaderButtonSel,
        CC_CALLBACK_1(MainMenu::switchToLeaderboards, this));

    leaderboardButton->setPosition(Vec2(startButton->getPositionX(),
        startButton->getPositionY()
            - (leaderboardButton->getContentSize().height) * SCALE_FACTOR));
    leaderboardButton->setScale(SCALE_FACTOR / 2);

    Menu* menu = Menu::create(startButton, leaderboardButton, NULL);
    menu->setPosition(Vec2::ZERO);

    // Backdrop
    for (int i = 0; i < 5; i++) {
        Sprite* backgroundImg = Sprite::create("basic_day.png");
        backgroundImg->setAnchorPoint(Point::ZERO);
        backgroundImg->setPosition(
            i * (backgroundImg->getContentSize().width * SCALE_FACTOR), 0);
        backgroundImg->getTexture()->setAliasTexParameters();
        backgroundImg->setScale(SCALE_FACTOR);
        this->addChild(backgroundImg, 0);
    }

    this->addChild(menu, 1);
    this->addChild(gameTitle, 1);
    this->initAudio();
    this->preloadAudio();
    if (DEBUG_ENABLE_MUSIC_ON) {
        this->playBackgroundMusic();
    }

    return true;
}

void MainMenu::startGameCall(Ref* pSender) {
    Network::getInstance()->subscribe(JOIN_ROOM_KEY, this,
        [this](bool success, nlohmann::json json) {
            std::list<Pipe*> pipes;
            for (auto& j : json["stage"]["pipes"]) {
                pipes.push_back(Pipe::create(j.at("type").get<std::string>(),
                                             j.at("x").get<float>(),
                                             j.at("y").get<float>()));
            }

            Director::getInstance()
                ->getScheduler()
                ->performFunctionInCocosThread([pipes] {
                    auto director = Director::getInstance();
                    Scene* scene;
                    if (DEBUG_QUEUE_ON) {
                        scene = QueueScene::createScene();
                    } else {
                        scene = GameScene::createScene(pipes);
                    }
                    director->replaceScene(scene);
                });
        });

    Network::getInstance()->joinRoom();
}

void MainMenu::switchToLeaderboards(Ref* refPointer) {
    Director* director = Director::getInstance();
    Scene* scene = LeaderboardScene::createScene();

    director->replaceScene(scene);
}

void MainMenu::initAudio() {
    this->sceneAudio = CocosDenshion::SimpleAudioEngine::getInstance();
}

void MainMenu::preloadAudio() {
    this->sceneAudio->preloadBackgroundMusic("start_background.wav");
}

void MainMenu::playBackgroundMusic() {
    this->sceneAudio->playBackgroundMusic("start_background.wav", false);
}

void MainMenu::stopBackgroundMusic() {
    this->sceneAudio->stopBackgroundMusic();
}
