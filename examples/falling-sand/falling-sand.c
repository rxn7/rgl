#include <stdlib.h>
#include <time.h>

#include <rgl/rgl.h>
#include <rgl/rgl_input.h>
#include <rgl/rgl_sprite.h>

#define WORLD_WIDTH 512
#define WORLD_HEIGHT 512 
#define WORLD_SIZE WORLD_WIDTH * WORLD_HEIGHT
#define MAX_CHANGES_PER_TICK WORLD_SIZE

static const f32 TICK_INTERVAL = 1.f / 300.f;

enum {
        MAT_IDX_EMPTY = 0,
        MAT_IDX_SAND,
        MAT_IDX_STONE,
        MAT_COUNT,
};

typedef struct material_t {
        bool solid;
        rgl_color_t color;
} material_t;

typedef struct particle_t {
        u8 mat_idx;
} particle_t;

typedef struct change_t {
        u32 idx;
        u8 mat;
} change_t;

static void move_particle(u32 x, u32 y, u32 xo, u32 yo);
static void draw_particles();
static void tick();
static void randomize();
static void clear();

static void app_init();
static void app_quit();
static void app_update(f64 dt);
                         
#define get_particle_idx(x,y) (y) * WORLD_WIDTH + (x)
#define get_particle(x,y) particles[get_particle_idx(x,y)]
#define is_solid(x,y) materials[get_particle(x,y).mat_idx].solid

static b8 paused = false;
static b8 dirty = false;
static f32 tick_timer = 0;

static rgl_sprite_t sprite;
static rgl_texture_t texture;

static particle_t particles[WORLD_SIZE] = {0};
static change_t changes[MAX_CHANGES_PER_TICK] = {0};
static material_t materials[MAT_COUNT] = {0};

static u32 change_count = 0;

int main(int argc, const char **argv) {
        srand(time(0));

        rgl_app_desc_t desc = (rgl_app_desc_t){
                .title = "RGL | Falling Sand",
                .width = WORLD_WIDTH,
                .height = WORLD_HEIGHT,
                .update_f = app_update,
                .quit_f = app_quit,
                .init_f = app_init,
                .res_width = WORLD_WIDTH,
                .res_height = WORLD_HEIGHT, 
        };

        rgl_init(&desc);
}

static void app_update(f64 dt) {
        if(tick_timer >= TICK_INTERVAL) {
                tick_timer = 0;
                tick();
		printf("FPS: %f\n", 1.f/dt);
        } else {
                tick_timer += dt;
        }
        
        s32 mx, my; 
	rgl_get_cursor_pos(&mx, &my);

        mx *= (WORLD_WIDTH / (f32)g_data.width);
        my *= (WORLD_HEIGHT / (f32)g_data.height);

        if(mx >= 0 && mx < WORLD_WIDTH && my >= 0 && my < WORLD_HEIGHT) {
                if(rgl_is_key_pressed(RGL_KEY_1)) {
			dirty = true;
			get_particle((u32)mx, (u32)my).mat_idx = MAT_IDX_EMPTY;
		} else if(rgl_is_key_pressed(RGL_KEY_2)) {
			dirty = true;
			get_particle((u32)mx, (u32)my).mat_idx = MAT_IDX_STONE;
		} else if(rgl_is_key_pressed(RGL_KEY_3)) {
			dirty = true;
			get_particle((u32)mx, (u32)my).mat_idx = MAT_IDX_SAND;
		}
        }

        if(rgl_is_key_just_pressed(RGL_KEY_R)) randomize();
        if(rgl_is_key_just_pressed(RGL_KEY_C)) clear();
	if(rgl_is_key_just_pressed(RGL_KEY_ESC)) paused = !paused;

	rgl_sprite_render(&sprite);
}

static void app_init() {
        rgl_texture_initialize(&texture, WORLD_WIDTH, WORLD_HEIGHT);
	rgl_sprite_initialize(&sprite, &texture);

	rgl_set_vsync(false);

        materials[MAT_IDX_EMPTY] = (material_t){false, RGL_RGB(0, 0, 0)};
        materials[MAT_IDX_SAND] = (material_t){false, RGL_RGB(255, 255, 0)};
        materials[MAT_IDX_STONE] = (material_t){true, RGL_RGB(200, 200, 200)};

        randomize();
}

static void app_quit() {
        rgl_texture_destroy(&texture);
	rgl_sprite_destroy(&sprite);
}

static void draw_particles() {
        for(u32 i=0; i<WORLD_SIZE; ++i) {
		rgl_texture_fill_pixel_idx(&texture, i, materials[particles[i].mat_idx].color);
        }
}

static void move_particle(u32 x, u32 y, u32 xo, u32 yo) {
        if(change_count < MAX_CHANGES_PER_TICK-3) {
                u8 previous_mat_idx = get_particle(x+xo, y+yo).mat_idx;

                changes[change_count++] = (change_t){ get_particle_idx(x+xo, y+yo), get_particle(x,y).mat_idx };
                changes[change_count++] = (change_t){ get_particle_idx(x, y), previous_mat_idx };

                dirty = true;
        }
}

static void tick() {
	if(paused) return;

        particle_t *part;
        for(u32 x=0; x<WORLD_WIDTH; ++x) {
                for(u32 y=0; y<WORLD_HEIGHT; ++y) {
			if(y <= 0) continue;
                        part = &get_particle(x, y);

                        switch(part->mat_idx) {
                                case MAT_IDX_SAND: {
                                        if(get_particle(x, y-1).mat_idx == MAT_IDX_EMPTY) move_particle(x, y, 0, -1);
                                        else {
                                                bool check_left = rand() % 2; // 50% chance of checking the left side first.
                                                bool can_move_left = x > 0 && get_particle(x-1, y).mat_idx == MAT_IDX_EMPTY && get_particle(x-1, y-1).mat_idx == MAT_IDX_EMPTY; 
                                                bool can_move_right = x < WORLD_WIDTH-1 && get_particle(x+1, y).mat_idx == MAT_IDX_EMPTY && get_particle(x+1, y-1).mat_idx == MAT_IDX_EMPTY; 

                                                if((check_left || !can_move_right) && can_move_left) move_particle(x, y, -1, -1);
                                                else if(can_move_right) move_particle(x, y, +1, -1);
                                        } 
                                        break;
                                }
                        }
                }
        }

        change_t *change;
        for(u32 i=0; i<change_count; ++i) {
                change = &changes[i];
                particles[change->idx].mat_idx = change->mat;
        }

        change_count = 0;

        if(dirty) {
		rgl_texture_clear(&texture);
                draw_particles();
		rgl_texture_update(&texture);
        }

        dirty = false;
}

static void randomize() {
        dirty = true;

        for(u32 i=0; i<WORLD_SIZE; ++i) {
                particles[i].mat_idx = rand() % 10 < 2 ? (rand() % MAT_COUNT + 1) : MAT_IDX_EMPTY;
        }
}

static void clear() {
        dirty = true;

        for(u32 i=0; i<WORLD_SIZE; ++i) {
                particles[i].mat_idx = MAT_IDX_EMPTY;
        }
}
