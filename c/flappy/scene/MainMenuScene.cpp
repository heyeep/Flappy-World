#include "MainMenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

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
    playButton->getTexture()->setAliasTexParameters();
    playButtonSel->getTexture()->setAliasTexParameters();

    MenuItemSprite* startButton = MenuItemSprite::create(playButton,
        playButtonSel,
        CC_CALLBACK_1(MainMenu::startGameCall, this));
    startButton->setPosition(Vec2(gameTitle->getPositionX(),
        gameTitle->getPositionY()
            - (startButton->getContentSize().height * SCALE_FACTOR)));
    startButton->setScale(SCALE_FACTOR / 2);

    Menu* menu = Menu::create(startButton, NULL);
    menu->setPosition(Vec2::ZERO);

    // Backdrop
    for (int i = 0; i < 5; i++) {
        Sprite* backgroundImg = Sprite::create("basic_day.png");
        backgroundImg->setAnchorPoint(Point::ZERO);
        backgroundImg->setPosition(
            i * (backgroundImg->getContentSize().width * SCALE_FACTOR),
            0);
        backgroundImg->getTexture()->setAliasTexParameters();
        backgroundImg->setScale(SCALE_FACTOR);
        this->addChild(backgroundImg, 0);
    }
    
    this->addChild(menu, 1);
    this->addChild(gameTitle, 1);
    this->initAudio();
    this->preloadAudio();
    this->playBackgroundMusic();

    return true;
}

void MainMenu::startGameCall(Ref* pSender) {
    Director* director = Director::getInstance();
    Scene* scene = GameScene::createScene();

    director->replaceScene(scene);
}

void MainMenu::initAudio() {
    this->sceneAudio = CocosDenshion::SimpleAudioEngine::getInstance();
}

void MainMenu::preloadAudio() {
    this->sceneAudio->preloadBackgroundMusic("start_background.mp3");
}

void MainMenu::playBackgroundMusic() {
    this->sceneAudio->playBackgroundMusic("start_background.mp3", false);
}

void MainMenu::stopBackgroundMusic() {
    this->sceneAudio->stopBackgroundMusic();
}