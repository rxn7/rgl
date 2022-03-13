/* Settings */
#define PLAYER_MOVE_SPEED 200
#define PLAYER_SPRITE_PATH "res/player.png"

#include <rgl/rgl.h> 

void app_init();
void app_init();
void app_quit();
void app_update(f32 dt);
void player_movement(f32 dt);

rgl_sprite_t *player;

int main(int argc, const char **argv) {
	rgl_app_desc_t desc = (rgl_app_desc_t) {
		.width = 640,
		.height = 480,
		.title = "RGL | Coc",
		.background_color = RGL_RGB(10, 100, 10),
		.update_f = app_update,
		.init_f = app_init,
		.quit_f = app_quit,
	};

	rgl_init(&desc);
}

void app_init() {
	player = malloc(sizeof(rgl_sprite_t));

	player->texture = malloc(sizeof(rgl_texture_t));
	rgl_texture_load_from_file(player->texture, PLAYER_SPRITE_PATH, RGL_TEXTURE_FILTER_NONE);

	rgl_sprite_create(player);

	rgl_v2_setf(&player->size, 50);

	player->position.x = (f32)g_rgl_data->width / 2.f;
	player->position.y = (f32)g_rgl_data->height / 2.f;
}

void app_quit() {
	rgl_texture_destroy(player->texture);
	rgl_sprite_destroy(player);
}

void app_update(f32 dt) {
	player_movement(dt);
	rgl_sprite_render(player);
}

void player_movement(f32 dt) {
	v2 dir = {0};
	if(rgl_is_key_pressed(RGL_KEY_W)) dir.y--;
	if(rgl_is_key_pressed(RGL_KEY_S)) dir.y++;
	if(rgl_is_key_pressed(RGL_KEY_A)) dir.x--;
	if(rgl_is_key_pressed(RGL_KEY_D)) dir.x++;
	rgl_v2_mulf(&dir, dt * PLAYER_MOVE_SPEED, &dir);
	rgl_v2_add(&player->position, &dir, &player->position);

	/* Horizontal flip */
	if(dir.x != 0) {
		player->size.x = fabs(player->size.x) * (dir.x < 0 ? -1 : 1);
	}
}
