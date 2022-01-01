#include <stdlib.h>
#include <time.h>

#include <rgl/rgl.h>
#include <rgl/rgl_input.h>

#define WORLD_WIDTH 512
#define WORLD_HEIGHT 512 
#define WORLD_SIZE WORLD_WIDTH * WORLD_HEIGHT
#define MAX_CHANGES_PER_TICK WORLD_SIZE

static const f32 TICK_INTERVAL = 1.f / 90.f;

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
static void app_update(f32 dt);
                         
#define get_particle_idx(x,y) (y) * WORLD_WIDTH + (x)
#define get_particle(x,y) particles[get_particle_idx(x,y)]
#define is_solid(x,y) materials[get_particle(x,y).mat_idx].solid

static bool dirty = false;
static f32 tick_timer = 0;
static rgl_texture_t *buffer;

static particle_t particles[WORLD_SIZE] = {0};
static change_t changes[MAX_CHANGES_PER_TICK] = {0};
static material_t materials[MAT_COUNT] = {0};

static u32 change_last_idx = 0;

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

static void app_update(f32 dt) {
        dirty = false;

        if(tick_timer >= TICK_INTERVAL) {
                tick_timer = 0;
                tick();
        } else {
                tick_timer += dt;
        }
        
        f64 mx, my;
        rgl_get_cursor_pos(&mx, &my);

        u32 ww, wh;
        rgl_get_window_size(&ww, &wh);

        mx *= (WORLD_WIDTH / (f32)ww);
        my *= (WORLD_HEIGHT / (f32)wh);

        if(mx >= 0 && mx < WORLD_WIDTH && my >= 0 && my < WORLD_HEIGHT) {
                bool _dirty = true;

                if(rgl_is_key_pressed(GLFW_KEY_1))      get_particle((u32)mx, (u32)my).mat_idx = MAT_IDX_EMPTY;
                else if(rgl_is_key_pressed(GLFW_KEY_2)) get_particle((u32)mx, (u32)my).mat_idx = MAT_IDX_STONE;
                else if(rgl_is_key_pressed(GLFW_KEY_3)) get_particle((u32)mx, (u32)my).mat_idx = MAT_IDX_SAND;
                else _dirty = false;

                if(!dirty) dirty = _dirty;
        }

        if(rgl_is_key_pressed(GLFW_KEY_R)) randomize();
        if(rgl_is_key_pressed(GLFW_KEY_C)) clear();

        if(dirty) {
                rgl_texture_clear(buffer);
                draw_particles();
        }

        rgl_render_texture(buffer, true);
}

static void app_init() {
        buffer = rgl_texture_new(WORLD_WIDTH, WORLD_HEIGHT);

        materials[MAT_IDX_EMPTY] = (material_t){false, RGL_RGB(0, 0, 0)};
        materials[MAT_IDX_SAND] = (material_t){false, RGL_RGB(255, 255, 0)};
        materials[MAT_IDX_STONE] = (material_t){true, RGL_RGB(200, 200, 200)};

        randomize();
}

static void app_quit() {
        rgl_texture_free(buffer);
}

static void draw_particles() {
        particle_t *part; 
        for(u32 i=0; i<WORLD_SIZE; ++i) {
                part = &particles[i];
                if(part->mat_idx == MAT_IDX_EMPTY) {
                        continue;
                } 
                rgl_texture_fill(buffer, i, materials[part->mat_idx].color); 
        }
}

static void move_particle(u32 x, u32 y, u32 xo, u32 yo) {
        if(change_last_idx < MAX_CHANGES_PER_TICK-3) {
                u8 previous_mat_idx = get_particle(x+xo, y+yo).mat_idx;

                changes[change_last_idx++] = (change_t){ get_particle_idx(x+xo, y+yo), get_particle(x,y).mat_idx };
                changes[change_last_idx++] = (change_t){ get_particle_idx(x, y), previous_mat_idx };

                dirty = true;
        }
}

static void tick() {
        particle_t *part;
        for(u32 x=0; x<WORLD_WIDTH; ++x) {
                for(u32 y=0; y<WORLD_HEIGHT; ++y) {
			if(y <= 0) continue;
                        part = &get_particle(x, y);

                        switch(part->mat_idx) {
                                case MAT_IDX_SAND: {
                                        if(get_particle(x, y-1).mat_idx == MAT_IDX_EMPTY) move_particle(x, y, 0, -1);
                                        else {
                                                bool check_left = rand() % 2 == 0; // 50% chance of checking the left side first.
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
        for(u32 i=0; i<change_last_idx; ++i) {
                change = &changes[i];
                particles[change->idx].mat_idx = change->mat;
        }

        change_last_idx = 0;
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
