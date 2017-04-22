#include "MainMenuScene.h"
#include "SinglePlayerScene.h"
#include "MultiPlayerScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();
    Layer* layer = MainMenu::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool MainMenu::init()
{
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Label* gameTitle = Label::createWithSystemFont("FLAPPY WORLD", "Arial", 100);

    // To position a sprite using it's mid point
    gameTitle->setAnchorPoint(Vec2(0.5, 0.5));
    
    gameTitle->setPosition(Vec2(visibleSize.width/2,
                            visibleSize.height - gameTitle->getContentSize().height));

    MenuItemImage* singlePlayerButton = MenuItemImage::create("singleplayerbutton.png",
                                                    "singleplayerbutton-s.png",
                                                    CC_CALLBACK_1(MainMenu::callSinglePlayer, this));
    singlePlayerButton->setPosition(Vec2(gameTitle->getPositionX(),
                                         gameTitle->getPositionY() -
                                         singlePlayerButton->getContentSize().height));
    MenuItemImage* multiPlayerButton = MenuItemImage::create("multiplayerbutton.png",
                                                   "multiplayerbutton-s.png",
                                                   CC_CALLBACK_1(MainMenu::callMultiPlayer, this));
    multiPlayerButton->setPosition(Vec2(gameTitle->getPositionX(),
                                        gameTitle->getPositionY() - 
                                        multiPlayerButton->getContentSize().height * 2));
    Menu* menu = Menu::create(singlePlayerButton, multiPlayerButton, NULL);
    
    menu->setPosition(Vec2::ZERO);

    this->addChild(menu, 1);
    this->addChild(gameTitle, 1);

    return true;
}

// Call new scenes from here
void MainMenu::callSinglePlayer(Ref* pSender)
{
    Director* director = Director::getInstance();
    Scene* scene = SinglePlayerGame::createScene();
    
    director->replaceScene(scene);
}

void MainMenu::callMultiPlayer(Ref* pSender)
{
    auto director = Director::getInstance();
    auto scene = MultiPlayerGame::createScene();
    
    director->replaceScene(scene);
}
