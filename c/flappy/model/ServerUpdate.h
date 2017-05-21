#ifndef ServerUpdate_H
#define ServerUpdate_H

#include "json.hpp"
#include <string>

class ServerUpdate {
public:
    /**
     *  \brief The type of the update.
     *
     *  The Phoenix Channel Event.
     *
     *  \return std::string
     */
    virtual std::string getType() = 0;

    /**
     *  \brief The body of the update.
     *
     *  The Phoenix Channel Payload.
     *
     *  \return nlohmann::json
     */
    virtual nlohmann::json getPayload() = 0;
};

#endif // ServerUpdate_H
