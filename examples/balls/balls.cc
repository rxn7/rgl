#include <unistd.h>
#include <vector>
#include <thread>

extern "C" {
	#include <rgl/rgl.h>
}

#define RANDOM_INITIAL_VELOCITY 4000

#define MAX_RADIUS 40.f
#define MIN_RADIUS 5.f

#define BALL_COUNT 200
#define WIDTH 640
#define HEIGHT 480
#define MASS(b) (b->radius*10.0f)

enum collision_type_t : u8{
	COLLISION_WITH_BALL,
	COLLISION_WITH_WALL,
};

enum wall_type_t : bool {
	WALL_HORIZONTAL,
	WALL_VERTICAL,
};

struct ball_t {
	rgl_color_t color;
	vec2 pos;
	vec2 vel;
	vec2 accel;
	f32 radius;
};

struct collision_t {
	collision_type_t type;
	ball_t *a;

	union {
		ball_t *b;
		wall_type_t wall_type;
	};
};

void app_quit();
void app_update(f32 dt);
void app_init();

void init_balls();
void update_balls();
void draw_balls();
void add_ball(vec2 pos);
void update_thread_func();
bool is_point_in_ball(ball_t *ball, vec2 point);

u32 id_count = 0;
ball_t *selected_ball = NULL;
std::thread update_thread;

bool running = true;
std::vector<ball_t> vec_balls;

int main(int argc, const char **argv) {
        rgl_app_desc_t desc = (rgl_app_desc_t){
                .title = "RGL | Balls",
                .height = HEIGHT,
                .width = WIDTH,
                .init_f = app_init,
                .update_f = app_update,
                .quit_f = app_quit,
        };

        rgl_init(&desc);
}

void app_init() {
        srand(time(0));

	init_balls();
	update_thread = std::thread(update_thread_func);
}

void app_update(f32 dt) {
	bool left_pressed = rgl_is_button_pressed(RGL_MOUSE_LEFT);
	bool right_pressed = rgl_is_button_pressed(RGL_MOUSE_RIGHT);

	vec2 mouse_pos;
	rgl_get_cursor_pos(mouse_pos);

	if(left_pressed || right_pressed) {
		if(selected_ball == NULL) {
			for(ball_t &ball : vec_balls) {
				if(is_point_in_ball(&ball, mouse_pos)) {
					selected_ball = &ball;
					break;
				}
			}
		} else if(left_pressed){
			for(u8 i=0;i<2;++i) {
				selected_ball->accel[i] = 0;
				selected_ball->vel[i] = -5.0f * (selected_ball->pos[i] - mouse_pos[i]);
			}
		}
	} else {
		if(rgl_is_button_just_released(RGL_MOUSE_RIGHT) && selected_ball != NULL) {
			for(u8 i=0;i<2;++i) {
				selected_ball->accel[i] = 0;
				selected_ball->vel[i] = -5.0f * (mouse_pos[i] - selected_ball->pos[i]);
			}
		}

		selected_ball = NULL;
	}

	if(rgl_is_key_just_pressed(RGL_KEY_N)) {
		vec2 pos;
		rgl_get_cursor_pos(pos);

		add_ball(pos);
	}

	draw_balls();
}

void app_quit() {
	running = false;
	update_thread.join();
}

bool is_point_in_ball(ball_t *ball, vec2 point) {
	vec2 delta_pos;
	for(u8 i=0;i<2;++i) delta_pos[i] = ball->pos[i] - point[i];
	
	return delta_pos[0]*delta_pos[0] + delta_pos[1]*delta_pos[1] < (ball->radius * ball->radius);
}

void update_thread_func() {
	f32 now, last = rgl_get_time(), dt;

	std::vector<collision_t> vec_collisions;

	while(running) {
		vec_collisions.clear();

		now = rgl_get_time();
		dt = now - last;
		last = now;

		for(ball_t &ball : vec_balls) {
			for(u8 i=0; i<2; ++i) {
				ball.accel[i] = -ball.vel[i] * 0.9f; /* Drag */
				ball.vel[i] += ball.accel[i] * dt;
				ball.pos[i] += ball.vel[i] * dt;
			}

			if(ball.vel[0]*ball.vel[0] + ball.vel[1]*ball.vel[1] < 0.01f) {
				glm_vec2_copy((float *)VEC2_ZERO, ball.accel);
			}
		}

		u32 i=0; 

		/* Static colliisions */
		for(ball_t &ball : vec_balls) {
			u32 j=0;

			/* Wall collisions */
			/* Left wall */
			if(ball.pos[0] - ball.radius <= 0) {
				ball.pos[0] = ball.radius;
				vec_collisions.push_back({ COLLISION_WITH_WALL, &ball, { .wall_type = WALL_VERTICAL } });
			} 
			/* Right wall */
			else if(ball.pos[0] + ball.radius >= g_data.width) {
				ball.pos[0] = g_data.width - ball.radius;
				vec_collisions.push_back({ COLLISION_WITH_WALL, &ball, { .wall_type = WALL_VERTICAL } });
			}
			/* Bottom wall */
			if(ball.pos[1] - ball.radius <= 0) {
				ball.pos[1] = ball.radius;
				vec_collisions.push_back({ COLLISION_WITH_WALL, &ball, { .wall_type = WALL_HORIZONTAL } });
			}
			/* Top wall */
			else if(ball.pos[1] + ball.radius >= g_data.height) {
				ball.pos[1] = g_data.height - ball.radius;
				vec_collisions.push_back({ COLLISION_WITH_WALL, &ball, { .wall_type = WALL_HORIZONTAL } });
			}

			/* Overlap collisions */
			for(ball_t &target : vec_balls) {
				if(i == j++) continue; /* Ignore checking ball against itself */

				vec2 delta_pos;
				for(u8 i=0; i<2; ++i) delta_pos[i] = ball.pos[i] - target.pos[i];

				f32 radius_sum = ball.radius + target.radius;

				/* If balls overlap */
				if(delta_pos[0]*delta_pos[0] + delta_pos[1]*delta_pos[1] <= radius_sum * radius_sum) { 
					vec_collisions.push_back({ COLLISION_WITH_BALL, &ball, { .b = &target } });

					f32 dist = sqrtf(delta_pos[0]*delta_pos[0] + delta_pos[1]*delta_pos[1]);
					f32 overlap = (dist - ball.radius - target.radius) * 0.5f;

					for(u8 i=0; i<2; ++i) {
						ball.pos[i] -= overlap * delta_pos[i] / dist;
						target.pos[i] += overlap * delta_pos[i] / dist;
					}
				}
			}

			++i;
		}

		/* Dynamic collisions */
		for(collision_t &collision : vec_collisions) {
			if(collision.type == COLLISION_WITH_BALL) {
				ball_t *a = collision.a;
				ball_t *b = collision.b;

				vec2 delta_pos;
				for(u8 i=0; i<2; ++i) delta_pos[i] = a->pos[i] - b->pos[i];

				vec2 delta_vel;
				for(u8 i=0; i<2; ++i) delta_vel[i] = a->vel[i] - b->vel[i];

				f32 dist = sqrtf(delta_pos[0]*delta_pos[0] + delta_pos[1]*delta_pos[1]);

				vec2 normal;
				for(u8 i=0; i<2; ++i) normal[i] = (b->pos[i] - a->pos[i]) / dist;

				f32 p = 2.0f * (normal[0] * delta_vel[0] + normal[1] * delta_vel[1]) / (MASS(a) + MASS(b));

				for(u8 i=0; i<2; ++i) {
					a->vel[i] -= p * MASS(b) * normal[i];
					b->vel[i] += p * MASS(a) * normal[i];
				}
			} else {
				ball_t *ball = collision.a;

				// TODO: I'm not sure if this is accurate
				if(collision.wall_type == WALL_VERTICAL) ball->vel[0] *= -1;
				else ball->vel[1] *= -1;
			}
		}
	}
}

void draw_balls() {
	for(ball_t &ball : vec_balls) {
		rgl_immediate_draw_circle(ball.color, ball.pos, ball.radius);
	}

	if(selected_ball != NULL) {
		rgl_immediate_draw_circle(RGL_RED, selected_ball->pos, selected_ball->radius * 0.25f);

		vec2 mouse_pos;
		rgl_get_cursor_pos(mouse_pos);

		rgl_immediate_draw_line(RGL_BLUE, selected_ball->pos, mouse_pos, 5);
	}
}

void init_balls() {
	vec2 pos;
	for(u32 i=0; i<BALL_COUNT; ++i) {
		pos[0] = (f32)(rand() % WIDTH);
		pos[1] = (f32)(rand() % HEIGHT);

		add_ball(pos);
	}
}

void add_ball(vec2 pos) {
	static const f32 HALF_RANDOM_INITIAL_VELOCITY = RANDOM_INITIAL_VELOCITY * 0.5f;

	ball_t ball = {
		.color = { .rgb = { (u8)RAND_255, (u8)RAND_255, (u8)RAND_255 } },
		.radius = (MAX_RADIUS - MIN_RADIUS) * ((f32)rand() / RAND_MAX) + MIN_RADIUS,
	};

	for(u8 i=0;i<2;++i) ball.vel[i] = rand() % RANDOM_INITIAL_VELOCITY - HALF_RANDOM_INITIAL_VELOCITY;

	vec_balls.push_back(ball);
}
