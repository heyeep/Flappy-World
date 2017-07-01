#include "LeaderboardScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

LeaderboardScene::LeaderboardScene() {
}

LeaderboardScene::~LeaderboardScene() {
}

Scene* LeaderboardScene::createScene() {
    Scene* scene = Scene::create();
    Layer* layer = LeaderboardScene::create();

    scene->addChild(layer);

    return scene;
}

bool LeaderboardScene::init() {
    if (!Layer::init()) {
        return false;
    }

    this->windowSize = Director::getInstance()->getVisibleSize();
    this->connectToScoreboard();

    return true;
}

void LeaderboardScene::connectToScoreboard() {
    std::shared_ptr<Network> network = Network::getInstance();
    network->getLeaderboard(
        [this](bool success, nlohmann::json response) {
            if (success) {
                json scores = response["list"];

                Scheduler* scheduler = Director::getInstance()->getScheduler();
                scheduler->performFunctionInCocosThread(CC_CALLBACK_0(
                    LeaderboardScene::displayScores, this, scores));
            } else {
                CCLOG("Error: Could not connect to server...");

                // Had to change this up, because gotoMainMenu was changed to
                // work with the MenuBackButton.
                Director* director = Director::getInstance();
                Scene* scene = MainMenu::createScene();

                director->replaceScene(scene);
            }
        });
}

void LeaderboardScene::displayScores(nlohmann::json response) {
    Label* leader = Label::createWithSystemFont("Leaderboards", "Arial", 50);
    leader->setPosition(Vec2(windowSize.width / 2,
        windowSize.height - leader->getContentSize().height));

    for (int i = 0; i < response.size(); i++) {
        std::stringstream ss;
        ss << response[i]["name"] << " - (" << response[i]["score"] << ")";
        std::string str = ss.str();

        Label* name = Label::createWithSystemFont(str, "Arial", 25);
        name->setPosition(
            Vec2(windowSize.width / 2, (windowSize.height / 2) + (i * 25)));

        this->addChild(name);
    }

    Sprite* menuButton = Sprite::create("menu_button.png");
    Sprite* menuButtonSel = Sprite::create("menu_button-s.png");
    menuButton->getTexture()->setAliasTexParameters();
    menuButtonSel->getTexture()->setAliasTexParameters();

    MenuItemSprite* gotoMenuButton = MenuItemSprite::create(menuButton,
        menuButtonSel,
        CC_CALLBACK_1(LeaderboardScene::gotoMainMenu, this));
    gotoMenuButton->setPosition(Vec2(windowSize.width - gotoMenuButton->getContentSize().width, windowSize.height - gotoMenuButton->getContentSize().height));
    gotoMenuButton->setScale(SCALE_FACTOR / 2);

    Menu* menu = Menu::create(gotoMenuButton, NULL);
    menu->setPosition(Vec2::ZERO);

    this->addChild(menu);
    this->addChild(leader);
}

void LeaderboardScene::gotoMainMenu(Ref* refPointer) {
    Director* director = Director::getInstance();
    Scene* scene = MainMenu::createScene();

    director->replaceScene(scene);
}
