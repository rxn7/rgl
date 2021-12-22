#include <rgl/rgl.h>
#include <stdlib.h>
#include <time.h>

#define WORLD_WIDTH 256 
#define WORLD_HEIGHT 256 
#define WORLD_SIZE WORLD_WIDTH * WORLD_HEIGHT

enum {
        EMPTY = 0,
        SAND,
        MAT_COUNT,
};

typedef struct particle_t {
        u8 material;
        u8 next_material; /* FIXME this is absolutely terrible */
        f32 lifetime;
} particle_t;

static void move_particle(u32 x, u32 y, u32 xo, u32 yo);
static void draw_particles();
static void app_init();
static void app_quit();
static void app_update(f32 dt);

#define get_particle(x,y) particles[(y) * WORLD_WIDTH + (x)]

rgl_texture_t *buffer;
particle_t particles[WORLD_SIZE] = {0};
rgl_color_t colors[MAT_COUNT-1] = {
        RGL_RGB(255, 255, 0), 
};

int main(int argc, const char **argv) {
        srand(time(0));

        rgl_app_desc_t desc = (rgl_app_desc_t){
                .title = "Hello World!",
                .width = 960,
                .height = 640,
                .update_f = app_update,
                .quit_f = app_quit,
                .init_f = app_init,
                .res_width = WORLD_WIDTH,
                .res_height = WORLD_HEIGHT, 
        };

        rgl_init(&desc);
}

static void app_update(f32 dt) {
        rgl_texture_clear(buffer);

        particle_t *part;
        for(u32 x=0; x<WORLD_WIDTH; ++x) {
                for(u32 y=0; y<WORLD_HEIGHT; ++y) {
                        part = &get_particle(x, y);
                        switch(part->material) {
                                case SAND:
                                        if(y > 0) {
                                                if(get_particle(x, y-1).material == EMPTY) move_particle(x, y, 0, -1);
                                                else if(x > 0 && get_particle(x-1, y-1).material == EMPTY) move_particle(x, y, -1, -1);
                                                else if(x < WORLD_WIDTH && get_particle(x+1, y-1).material == EMPTY) move_particle(x, y, +1, -1);
                                        }

                                        break;
                        }
                }
        }

        /* This is absolutely fucking terrible way of doing this */
        for(u32 i=0; i<WORLD_SIZE; ++i) {
                part = &particles[i];
                part->material = part->next_material;
        }

        draw_particles();
}

static void app_init() {
        buffer = rgl_texture_new(WORLD_WIDTH, WORLD_HEIGHT);

        for(u32 i=0; i<WORLD_SIZE; ++i) {
                particles[i].next_material = rand() % 10 < 1 ? SAND : EMPTY;
        }
}

static void app_quit() {
        rgl_texture_free(buffer);
}

static void draw_particles() {
        particle_t *part; 
        for(u32 i=0; i<WORLD_SIZE; ++i) {
                part = &particles[i];
                if(part->material == EMPTY) {
                        continue;
                } 
                rgl_texture_fill(buffer, i, colors[part->material-1]); 
        }
        rgl_render_texture(buffer, true);
}

static void move_particle(u32 x, u32 y, u32 xo, u32 yo) {
        particle_t *part = &get_particle(x, y);
        u8 mat = part->material;
        part->next_material = EMPTY;
 
        get_particle(x+xo, y+yo).next_material = mat;
}
