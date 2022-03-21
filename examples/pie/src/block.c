#include "block.h"

void 
pieBlockCreate(pieBlock *block, f32 pos, f32 size, f32 vel, f32 mass) {
	block->position = pos;
	block->size = size;
	block->velocity = vel;
	block->mass = mass;
}

void
pieBlockUpdate(pieBlock *block, f32 dt) {
	block->position += block->velocity * dt;
}

void
pieBlockDraw(pieBlock *block) {
	f32 y = _rgl_data->camera->bottom - block->size * 0.5f;
	rglDrawRect(RGL_RED, (rglV2){block->position, y}, (rglV2){block->size, block->size});
}

void
pieBlockBounce(pieBlock *a, pieBlock *b) {
	f32 total_mass = a->mass + b->mass;
	a->velocity = (a->mass - b->mass) * a->velocity / total_mass + 2.0f * b->mass * b->velocity / total_mass;
}

b8
pieBlockWallCollisionCheck(pieBlock *a) {
	if(a->position - a->size * 0.5f <= _rgl_data->camera->left) {
		a->velocity *= -1.0;
		return true;
	}

	return false;
}

b8 
pieBlocksCollide(pieBlock *a, pieBlock *b) {
	return a->position + a->size * 0.5f >= b->position - b->size * 0.5f;
}
