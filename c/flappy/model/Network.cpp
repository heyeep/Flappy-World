#include "Network.h"
#include "PhxChannel.h"
#include "PhxPush.h"
#include "PhxSocket.h"
#include "easylogging++.h"
#include <iostream>

std::shared_ptr<Network> Network::getInstance() {
    static std::shared_ptr<Network> instance = std::make_shared<Network>();
    return instance;
}

void Network::start() {
    std::shared_ptr<PhxSocket::PhxSocket> sk
        = std::make_shared<PhxSocket::PhxSocket>(
            "ws://localhost:4000/socket/websocket", 1);
    sk->setDelegate(this->shared_from_this());

    // Instantiate the PhxChannel first before connecting on the socket.
    // This is because the connection can happen before the channel
    // is done instantiating.
    sk->connect();

    this->socket = std::move(sk);
}

void Network::phxSocketDidOpen() {
    LOG(INFO) << "phxSocketDidOpen";
}

void Network::phxSocketDidClose(const std::string& event) {
    LOG(INFO) << "phxSocketDidClose" << std::endl;
}

void Network::phxSocketDidReceiveError(const std::string& error) {
    LOG(INFO) << "phxSocketDidReceiveError" << std::endl;
}

void Network::joinRoom() {
    if (!this->socket->isConnected()) {
        publish(JOIN_ROOM_KEY, false, nullptr);
        return;
    }

    // FIXME: Remove the :1 and represent it with a 'user id'.
    // This problem occurs in more than one place.
    this->roomChannel = std::make_shared<PhxChannel::PhxChannel>(
        this->socket, "room:1", std::map<std::string, std::string>());
    this->roomChannel->bootstrap();
    this->roomChannel->join()
        ->onReceive("ok",
            [this](nlohmann::json json) {
                LOG(INFO) << "Received OK on join:" << json.dump() << std::endl;
            })
        ->onReceive("error", [this](nlohmann::json error) {
            LOG(INFO) << "Error joining: " << error << std::endl;
            publish(JOIN_ROOM_KEY, false, error);
        });

    this->roomChannel->onEvent(
        "start", [this](nlohmann::json json, int64_t ref) {
            LOG(INFO) << "Received START on join:" << json.dump() << std::endl;
            publish(JOIN_ROOM_KEY, true, json);
        });

    this->roomChannel->onEvent(
        "new_player_joined", [](nlohmann::json json, int64_t ref) {
            LOG(INFO) << "Received NEW_PLAYER_JOINED message: " << json.dump()
                      << std::endl;
        });

    this->roomChannel->onEvent(
        "coordinates", [this](nlohmann::json message, int64_t ref) {
            this->publish(COORDINATES_UPDATE_KEY, true, message);
        });
}

void Network::getLeaderboard() {
    if (!this->socket->isConnected()) {
        publish(GET_LEADERBOARD, false, nullptr);
        return;
    }

    // FIXME: Remove the :1 and represent it with a 'user id'.
    // This problem occurs in more than one place.
    std::shared_ptr<PhxChannel> leaderBoardChannel
        = std::make_shared<PhxChannel::PhxChannel>(this->socket,
            "leaderboard:1",
            std::map<std::string, std::string>());
    leaderBoardChannel->bootstrap();
    leaderBoardChannel->join()
        ->onReceive("ok",
            [this](nlohmann::json json) {
                LOG(INFO) << "Received OK on join:" << json.dump() << std::endl;
                publish(GET_LEADERBOARD, true, json);
            })
        ->onReceive("error", [this](nlohmann::json error) {
            LOG(INFO) << "Error joining: " << error << std::endl;
            publish(GET_LEADERBOARD, false, nullptr);
        });
}

void Network::updateServer(std::shared_ptr<ServerUpdate> update) {
    if (!this->socket->isConnected()) {
        // Drop the update.
        return;
    }

    if (this->roomChannel) {
        this->roomChannel->pushEvent(update->getType(), update->getPayload());
    }
}

void Network::subscribe(
    const std::string& key, void* ref, PubSubCallback callback) {

    std::tuple<void*, PubSubCallback> refAndCallback(ref, callback);

    if (this->subscriberMap.find(key) == this->subscriberMap.end()) {
        std::vector<std::tuple<void*, PubSubCallback>> arr;
        arr.push_back(refAndCallback);
        this->subscriberMap[key] = arr;
    } else {
        std::vector<std::tuple<void*, PubSubCallback>> arr
            = this->subscriberMap[key];
        arr.push_back(refAndCallback);
    }
}

void Network::unsubscribe(const std::string& key, void* ref) {
    if (this->subscriberMap.find(key) == this->subscriberMap.end()) {
        return;
    }

    std::vector<std::tuple<void*, PubSubCallback>> arr
        = this->subscriberMap[key];

    int foundPosition = -1;

    for (int i = 0; i < arr.size(); i++) {
        std::tuple<void*, PubSubCallback> tuple = arr[i];
        if (std::get<0>(tuple) == ref) {
            foundPosition = i;
            break;
        }
    }

    if (foundPosition >= 0) {
        arr.erase(arr.begin() + foundPosition);
    }
}

void Network::publish(
    const std::string& key, bool success, nlohmann::json json) {
    std::vector<std::tuple<void*, PubSubCallback>> subscribers
        = this->subscriberMap[key];
    for (int i = 0; i < subscribers.size(); i++) {
        PubSubCallback cb = std::get<1>(subscribers[i]);
        cb(true, json);
    }
}
