#ifndef Network_H
#define Network_H

#include "PhxSocket.h"
#include "Pipe.h"
#include "ServerUpdate.h"
#include <memory>

// Event Keys
#define COORDINATES_UPDATE_KEY "coordinates_update"
#define JOIN_ROOM_KEY "join_room"
#define GET_LEADERBOARD "get_leaderboard"

/*!< Typedef for callback used in Network pubsub. */
using PubSubCallback = std::function<void(bool success, nlohmann::json json)>;

class PhxChannel;

class Network : public std::enable_shared_from_this<PhxSocketDelegate>,
                public PhxSocketDelegate {

private:
    // PhxSocketDelegate
    void phxSocketDidOpen();
    void phxSocketDidClose(const std::string& event);
    void phxSocketDidReceiveError(const std::string& error);
    // PhxSocketDelegate

    /*!< This is the channel for the current room the player has joined. */
    std::shared_ptr<PhxChannel> roomChannel;

    /*!< The socket used by the various Phoenix Channels game uses. */
    std::shared_ptr<PhxSocket> socket;

    /*!< Map that maps events (keys) to array of callbacks. */
    std::unordered_map<std::string,
        std::vector<std::tuple<void*, PubSubCallback>>>
        subscriberMap;

public:
    /**
     *  \brief Network Singleton.
     *
     *  Return a Network singleton.
     *
     *  \return Network
     */
    static std::shared_ptr<Network> getInstance();

    /**
      *  \brief Trigger start of Network connection.
      *
      *  Detailed description
      *
      *  \return return type
      */
    void start();

    /**
     * /brief Join Stage operation
     *
     * /return void
     */
    void joinRoom();

    /**
     *  \brief Get Leaderboard results.
     *
     *  Get Leaderboard results from server and return results in callback.
     *
     *  \return void
     */
    void getLeaderboard();

    /**
     *  \brief Push an update to channel.
     *
     *  Pushes an update to a Phoenix Channel.
     *
     *  \param update to push to server.
     *  \return void
     */
    void updateServer(std::shared_ptr<ServerUpdate> update);

    /**
     *  \brief Pubsub entry point.
     *
     *  Allow callers to subscribe to event designated by key. When event is
     *  triggered, callback is called.
     *
     *  \param key to subscribe to.
     *  \param ref used to subscribe.
     *  \param callback to run when event is triggered.
     *  \return void
     */
    void subscribe(const std::string& key, void* ref, PubSubCallback callback);

    /**
     *  \brief Unsubscribe listener.
     *
     *  Detailed description
     *
     *  \param to unsubscribe from.
     *  \param ref to unsubscribe.
     *  \return void
     */
    void unsubscribe(const std::string& key, void* ref);

    /**
     *  /brief publish to topic
     *
     *  Detailed description
     *
     *  /param key to publish to
     *  /param success whether or to indicate successful callback
     *  /param json the payload returned in publish
     */
    void publish(const std::string& key, bool success, nlohmann::json json);
};

#endif // Network_H
