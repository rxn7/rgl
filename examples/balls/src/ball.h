#ifndef __BALL_H
#define __BALL_H

#include "./common.h"

struct Ball {
	rglColor color;
	rglV2 position;
	rglV2 velocity;
	f32 radius;
	f32 last_sound_played_time;

	Ball(rglV2 position);
	b8 isPointInside(rglV2 point);
}; 

b8 isPointInsideBall(Ball *ball, rglV2 point);

#endif /* __BALL_H */
