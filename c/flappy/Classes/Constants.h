#ifndef Constants_h
#define Constants_h

#define SCALE_FACTOR 5.0
#define Z_BOTTOM_LAYER 0
#define Z_MIDDLE_LAYER 1
#define Z_TOP_LAYER 2
#define Z_HUD_LAYER 3
#define BIRD_SPEED 3
#define BIRD_VELOCITY Vec2(0, 300)
#define PLAYER_QUEUE_MAX 4
#define DEFAULT_QUEUE_TIME 10.0f
#define TAG_PLAYER 1
#define TAG_PIPE 2
#define TAG_POINTS 3
#define TAG_COIN 4

#define BITMASK_CONTACT_DEFAULT 0xFFF
#define BITMASK_CATEGORY_DEFAULT 0x02
#define BITMASK_COLLISION_DEFAULT 0xF1
#define BITMASK_COLLISION_PASSABLE 0xF2
#define BITMASK_COLLISION_DENSE 1

#define DEFAULT_PIPE_X_SPACING 400.0f
#define DEFAULT_PIPE_Y_SPACING 200.0f

// Debug Toggles
#define DEBUG_AUTOSTART_ON false
#define DEBUG_DEATH_ON false
#define DEBUG_QUEUE_ON false
#define DEBUG_ENABLE_MUSIC_ON false

#endif /* Constants_h */
