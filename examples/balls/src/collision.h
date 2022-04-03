#ifndef __COLLISION_H
#define __COLLISION_H

#include "./ball.h"

struct Collision {
	enum : b8 {
		COLLISION_W_WALL,
		COLLISION_W_BALL,
	} type;

	Ball *a;

	union {
		rglV2 position;
		Ball *b;
	};
};

#endif
