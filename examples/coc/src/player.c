#include "player.h"
#include "common.h"

static rglTexture _texture = {0};

static void player_movement(player_t *player, f32 dt);

void
player_create(player_t *player, const char *texture_path) {
	rglTextureLoadFromFile(&_texture, texture_path, RGL_TEXTURE_FILTER_NONE);
	rglSpriteCreate(&player->sprite, &_texture);

	player->sprite.size.x = 44;
	player->sprite.size.y = 88;

	player->sprite.position.x = (f32)_rgl_data->width / 2.f;
	player->sprite.position.y = (f32)_rgl_data->height / 2.f;

	rglV2 frame_size = (rglV2){22,44};
	f32 seconds_per_frame = 0.15f;

	player->animations = malloc(PLAYER_ANIM_COUNT * sizeof(rglSpriteAnimation));
	rglSpriteAnimationCreate(&player->animations[IDLE], player->sprite.texture, (u16[]){1}, 1, 0.2f, frame_size, true);
	rglSpriteAnimationCreate(&player->animations[MOVE_DOWN], player->sprite.texture, (u16[]){0, 1, 2, 1},  4, seconds_per_frame, frame_size, true);
	rglSpriteAnimationCreate(&player->animations[MOVE_LEFT], player->sprite.texture, (u16[]){3, 4, 5, 4},  4, seconds_per_frame, frame_size, true);
	rglSpriteAnimationCreate(&player->animations[MOVE_RIGHT], player->sprite.texture, (u16[]){6, 7, 8, 7}, 4, seconds_per_frame, frame_size, true);
	rglSpriteAnimationCreate(&player->animations[MOVE_UP], player->sprite.texture, (u16[]){9, 10, 11, 10}, 4, seconds_per_frame, frame_size, true);

	rglSpriteAnimatorCreate(&player->animator, &player->sprite, &player->animations[IDLE]);

	player_play_animation(player, IDLE);
}

void
player_destroy(player_t *player) {
	free(player->animations);

	rglTextureDestroy(player->sprite.texture);
	rglSpriteDestroy(&player->sprite);
}

void
player_update(player_t *player, f32 dt) {
	player_movement(player, dt);
	rglSpriteAnimatorUpdate(&player->animator, dt);
}

static void
player_movement(player_t *player, f32 dt) {
	rglV2Zero(&player->move_dir);
	if(rglIsKeyPressed(RGL_KEY_W)) player->move_dir.y--;
	if(rglIsKeyPressed(RGL_KEY_S)) player->move_dir.y++;
	if(rglIsKeyPressed(RGL_KEY_A)) player->move_dir.x--;
	if(rglIsKeyPressed(RGL_KEY_D)) player->move_dir.x++;

	if(player->move_dir.x != 0 || player->move_dir.y != 0) {
		if(player->move_dir.y > 0.0f) player_play_animation(player, MOVE_DOWN);
		else if(player->move_dir.y < 0.0f) player_play_animation(player, MOVE_UP);
		else if(player->move_dir.x > 0.0f) player_play_animation(player, MOVE_RIGHT);
		else if(player->move_dir.x < 0.0f) player_play_animation(player, MOVE_LEFT);

		rglV2Normalize(&player->move_dir, &player->move_dir);
		rglV2Mulf(&player->move_dir, dt * PLAYER_MOVE_SPEED, &player->move_dir);
		rglV2Add(&player->sprite.position, &player->move_dir, &player->sprite.position);
	} else {
		player_play_animation(player, IDLE);
	}
}

void
player_draw(player_t *player) {
	rglSpriteRender(&player->sprite);
}

void 
player_play_animation(player_t *player, player_animation_t anim) {
	rglSpriteAnimatorPlayNow(&player->animator, &player->animations[anim], false);
}
