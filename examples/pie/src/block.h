#ifndef __BLOCK_H
#define __BLOCK_H

#include <rgl/rgl.h>

typedef struct pieBlock {
	f32 position;
	f32 size;
	f32 velocity;
	f32 mass;
} pieBlock;

void pieBlockCreate(pieBlock *block, f32 pos, f32 size, f32 vel, f32 mass);
void pieBlockUpdate(pieBlock *block, f32 dt);
void pieBlockDraw(pieBlock *block);
void pieBlockBounce(pieBlock *a, pieBlock *b);
b8 pieBlockWallCollisionCheck(pieBlock *a);
b8 pieBlocksCollide(pieBlock *a, pieBlock *b);

#endif /* __BLOCK_H */
