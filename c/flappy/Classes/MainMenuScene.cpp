#include "MainMenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* MainMenu::createScene() {
    // 'scene' is an autorelease object
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
    Label* gameTitle
        = Label::createWithSystemFont("Flappy World", "Arial", 100);

    // To position a sprite using it's mid point
    gameTitle->setAnchorPoint(Vec2(0.5, 0.5));
    gameTitle->setPosition(Vec2(visibleSize.width / 2,
        visibleSize.height - gameTitle->getContentSize().height));

    MenuItemImage* startButton = MenuItemImage::create("startgame.png",
        "startgame-s.png",
        CC_CALLBACK_1(MainMenu::startGameCall, this));
    startButton->setPosition(Vec2(gameTitle->getPositionX(),
        gameTitle->getPositionY() - startButton->getContentSize().height));

    Menu* menu = Menu::create(startButton, NULL);
    menu->setPosition(Vec2::ZERO);

    this->addChild(menu, 1);
    this->addChild(gameTitle, 1);
    this->setAudio();
    this->preloadAudio();
    this->playBackgroundMusic();

    return true;
}

void MainMenu::startGameCall(Ref* pSender) {
    Director* director = Director::getInstance();
    Scene* scene = GameScene::createScene();

    director->replaceScene(scene);
}

void MainMenu::setAudio() {
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
