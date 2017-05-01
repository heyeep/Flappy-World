#ifndef Network_H
#define Network_H

#include "PhxSocket.h"
#include <memory>

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

public:
    /**
      *  \brief Trigger start of Network connection.
      *
      *  Detailed description
      *
      *  \param param
      *  \return return type
      */
    void start();
};

#endif // Network_H
