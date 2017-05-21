#include "CoordinateUpdate.h"

CoordinateUpdate::CoordinateUpdate(int serverId, int x, int y, int vx, int vy) {
    this->serverId = serverId;
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
}

std::string CoordinateUpdate::getType() {
    return "coordinates";
}

nlohmann::json CoordinateUpdate::getPayload() {
    // clang-format off
    return {
         { "server_id", this->serverId },
         { "x", this->x },
         { "y", this->y },
         { "lv_x", this->vx },
         { "lv_y", this->vy }
    };
    // clang-format on
}
