#include "Network.h"
#include "PhxChannel.h"
#include "PhxPush.h"
#include "PhxSocket.h"
#include "easylogging++.h"
#include <iostream>

void Network::start() {
    std::shared_ptr<PhxSocket::PhxSocket> socket
        = std::make_shared<PhxSocket::PhxSocket>(
            "ws://localhost:4000/socket/websocket", 1);
    socket->setDelegate(this->shared_from_this());

    this->channel = std::make_shared<PhxChannel::PhxChannel>(
        socket, "room:1", std::map<std::string, std::string>());
    this->channel->bootstrap();

    // Instantiate the PhxChannel first before connecting on the socket.
    // This is because the connection can happen before the channel
    // is done instantiating.
    socket->connect();
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
