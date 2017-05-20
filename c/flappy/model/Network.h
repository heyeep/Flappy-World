#ifndef Network_H
#define Network_H

#include "PhxSocket.h"
#include <memory>

/*!< Callback used for getting Leaderboard results.
  If able to retrieve results, success will be true. */
using GetLeaderBoardCallback
    = std::function<void(bool success, nlohmann::json json)>;

class PhxChannel;

class Network : public std::enable_shared_from_this<PhxSocketDelegate>,
                public PhxSocketDelegate {

private:
    // PhxSocketDelegate
    void phxSocketDidOpen();
    void phxSocketDidClose(const std::string& event);
    void phxSocketDidReceiveError(const std::string& error);
    // PhxSocketDelegate

    /*!< This is the main entry point of communication over Phoenix Channels. */
    std::shared_ptr<PhxChannel> channel;

    /*!< The socket used by the various Phoenix Channels game uses. */
    std::shared_ptr<PhxSocket> socket;

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
      *  \param param
      *  \return return type
      */
    void start();

    /**
     *  \brief Get Leaderboard results.
     *
     *  Get Leaderboard results from server and return results in callback.
     *
     *  \param callback The callback called when results are retrieved.
     *  \return void
     */
    void getLeaderboard(GetLeaderBoardCallback callback);
};

#endif // Network_H
