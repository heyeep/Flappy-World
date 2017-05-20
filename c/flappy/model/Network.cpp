#include "Network.h"
#include "PhxChannel.h"
#include "PhxPush.h"
#include "PhxSocket.h"
#include "easylogging++.h"
#include <iostream>

void Network::start() {
    std::shared_ptr<PhxSocket::PhxSocket> sk
        = std::make_shared<PhxSocket::PhxSocket>(
            "ws://localhost:4000/socket/websocket", 1);
    sk->setDelegate(this->shared_from_this());

    this->channel = std::make_shared<PhxChannel::PhxChannel>(
        sk, "room:1", std::map<std::string, std::string>());
    this->channel->bootstrap();

    // Instantiate the PhxChannel first before connecting on the socket.
    // This is because the connection can happen before the channel
    // is done instantiating.
    sk->connect();

    this->socket = std::move(sk);
}

void Network::phxSocketDidOpen() {
    LOG(INFO) << "phxSocketDidOpen";
    this->channel->join()
        ->onReceive("ok",
            [](nlohmann::json json) {
                LOG(INFO) << "Received OK on join:" << json.dump() << std::endl;
            })
        ->onReceive("error", [](nlohmann::json error) {
            LOG(INFO) << "Error joining: " << error << std::endl;
        });
}

void Network::phxSocketDidClose(const std::string& event) {
    LOG(INFO) << "phxSocketDidClose" << std::endl;
}

void Network::phxSocketDidReceiveError(const std::string& error) {
    LOG(INFO) << "phxSocketDidReceiveError" << std::endl;
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
