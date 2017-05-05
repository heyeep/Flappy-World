#ifndef Constants_h
#define Constants_h

#define SCALE_FACTOR 5.0
#define GAME_GRAVITY Vec2(0.0f, -600.0f)
#define Z_BOTTOM_LAYER 0
#define Z_MIDDLE_LAYER 1
#define Z_TOP_LAYER 2
#define Z_HUD_LAYER 3
#define BIRD_SPEED 3
#define BIRD_VELOCITY Vec2(0, 300)
#define BIRD_PHYSICS_MATERIAL PhysicsMaterial(0, 1, 0)

// Debug Toggles
#define DEBUG_AUTOSTART_ON true
#define DEBUG_DEATH_ON true
#define DEBUG_DRAW_DEBUG_MASK false

#endif /* Constants_h */
