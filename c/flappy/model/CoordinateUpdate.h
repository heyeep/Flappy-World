#ifndef CoordinateUpdate_H
#define CoordinateUpdate_H

#include "ServerUpdate.h"
#include "json.hpp"

class CoordinateUpdate : public ServerUpdate {
public:
    /*!< The id representing the player bird. */
    int serverId;
    /*!< x coordinate. */
    int x;
    /*!< y coordinate. */
    int y;
    /*!< Velocity in the x direction. */
    int vx;
    /*!< Velocity in the y direction. */
    int vy;

    CoordinateUpdate(int serverId, int x, int y, int vx, int vy);
    std::string getType();
    nlohmann::json getPayload();
    static CoordinateUpdate fromPayload(nlohmann::json payload);
};

#endif // CoordinateUpdate_H
