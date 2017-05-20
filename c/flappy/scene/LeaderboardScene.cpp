#include "LeaderboardScene.h"
#include "MainMenuScene.h"

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
    this->network = Network::getInstance();
    this->connectToScoreboard();

    return true;
}

void LeaderboardScene::connectToScoreboard() {
    this->network->getLeaderboard(
        [this](bool success, nlohmann::json response) {
            if (success) {
                json scores = response["list"];

                Scheduler* scheduler = Director::getInstance()->getScheduler();
                scheduler->performFunctionInCocosThread(CC_CALLBACK_0(
                    LeaderboardScene::displayScores, this, scores));
            } else {
                CCLOG("Error: Could not connect to server...");
                Scheduler* scheduler = Director::getInstance()->getScheduler();
                scheduler->performFunctionInCocosThread(
                    CC_CALLBACK_0(LeaderboardScene::gotoMainMenu, this));
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
    this->addChild(leader);
}

void LeaderboardScene::gotoMainMenu() {
    Director* director = Director::getInstance();
    Scene* scene = MainMenu::createScene();

    director->replaceScene(scene);
}
