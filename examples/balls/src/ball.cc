#include "./ball.h"

Ball::Ball(rglV2 position): position(position){
	this->radius = RGL_RAND_RANGE_F32(MIN_RADIUS, MAX_RADIUS);
	this->velocity = {0,0};
	this->color = RGL_COLOR_RAND;
}

b8 Ball::isPointInside(rglV2 point) {
	rglV2 delta;
	rglV2Sub(&this->position, &point, &delta);

	return delta.x*delta.x + delta.y*delta.y <= this->radius*this->radius;
}
