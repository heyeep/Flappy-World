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

void Network::joinRoom(JoinRoomCallback callback) {
    if (!this->socket->isConnected()) {
        callback(false, nullptr);
        return;
    }

    // FIXME: Remove the :1 and represent it with a 'user id'.
    // This problem occurs in more than one place.
    this->roomChannel = std::make_shared<PhxChannel::PhxChannel>(
        this->socket, "room:1", std::map<std::string, std::string>());
    this->roomChannel->bootstrap();
    this->roomChannel->join()
        ->onReceive("ok",
            [callback](nlohmann::json json) {
                LOG(INFO) << "Received OK on join:" << json.dump() << std::endl;
            })
        ->onReceive("error", [callback](nlohmann::json error) {
            LOG(INFO) << "Error joining: " << error << std::endl;
            callback(false, nullptr);
        });

    this->roomChannel->onEvent(
        "start", [callback](nlohmann::json json, int64_t ref) {
            LOG(INFO) << "Received START on join:" << json.dump() << std::endl;
            std::list<Pipe*> pipes;
            for (auto& j : json["stage"]["pipes"]) {
                pipes.push_back(Pipe::create(j.at("type").get<std::string>(),
                    j.at("x").get<float>(),
                    j.at("y").get<float>()));
                }
            callback(true, json);
        });

    this->roomChannel->onEvent(
        "new_player_joined", [callback](nlohmann::json json, int64_t ref) {
            LOG(INFO) << "Received NEW_PLAYER_JOINED message: " << json.dump() << std::endl;
        });

    this->roomChannel->onEvent(
        "coordinates", [this](nlohmann::json message, int64_t ref) {
            std::vector<PubSubCallback> arr
                = this->subscriberMap[COORDINATES_UPDATE_KEY];
            for (int i = 0; i < arr.size(); i++) {
                PubSubCallback cb = arr[i];
                cb(true, message);
            }
        });
}

void Network::getLeaderboard(GetLeaderBoardCallback callback) {
    if (!this->socket->isConnected()) {
        callback(false, nullptr);
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
            [callback](nlohmann::json json) {
                LOG(INFO) << "Received OK on join:" << json.dump() << std::endl;
                callback(true, json);
            })
        ->onReceive("error", [callback](nlohmann::json error) {
            LOG(INFO) << "Error joining: " << error << std::endl;
            callback(false, nullptr);
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

void Network::subscribe(const std::string& key, PubSubCallback callback) {
    if (this->subscriberMap.find(key) == this->subscriberMap.end()) {
        std::vector<PubSubCallback> arr;
        arr.push_back(callback);
        this->subscriberMap[key] = arr;
    } else {
        std::vector<PubSubCallback> arr = this->subscriberMap[key];
        arr.push_back(callback);
    }
}
