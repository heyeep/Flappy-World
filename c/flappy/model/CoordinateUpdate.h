#ifndef CoordinateUpdate_H
#define CoordinateUpdate_H

#include "ServerUpdate.h"
#include "json.hpp"

class CoordinateUpdate : public ServerUpdate {
private:
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

public:
    CoordinateUpdate(int serverId, int x, int y, int vx, int vy);
    std::string getType();
    nlohmann::json getPayload();
};

#endif // CoordinateUpdate_H
