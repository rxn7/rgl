#include <rgl/rgl.h>

#define RANDOM_INIT_VEL 2000
#define MAX_RADIUS 25.f
#define MIN_RADIUS 10.f
#define GRAVITY 980
#define FRICTION 0.99f
#define START_BALL_COUNT 500

#define MIN_TIME_BETWEEN_BOUNCE_SOUNDS 0.1f
#define BOUNCE_AUDIO_SOURCE_COUNT 20
#define CLICK_SOUND_PATH "res/click.ogg"
#define BOUNCE_SOUND_PATH "res/bounce.ogg"
