#include <vector> 
#include <thread>

#define RGL_DEBUG
extern "C" { 
	#include <rgl/rgl.h>
}

/* Settings */
#define RANDOM_INITIAL_VELOCITY 2000
#define MAX_RADIUS 20.f
#define MIN_RADIUS 15.f
#define GRAVITY 980
#define FRICTION 0.99f
#define START_BALL_COUNT 100

struct ball_t {
	rgl_audio_source_t audio_source;
	rgl_color_t color;
	v2 pos;
	v2 vel;
	f32 radius;
};
struct collision_t {
	enum : b8 {
		COLLISION_W_WALL,
		COLLISION_W_BALL,
	} type;

	ball_t *a;

	union {
		v2 pos;
		ball_t *b;
	};
};

void app_quit();
void app_update(f32 dt);
void app_init();

void init_balls();
void update_balls();
void draw_balls();
void add_ball(v2 pos);
void thr_physics_func();
b8 is_point_in_ball(ball_t *ball, v2 point);

std::vector<ball_t> vec_balls;
std::thread thr_physics;
ball_t *selected_ball = NULL;
b8 gravity = false;
b8 running = true;
rgl_audio_buffer_t *bounce_audio_buffer;

int main(int argc, const char **argv) {
        rgl_app_desc_t desc = {
                .title = "RGL | Balls",
                .height = 640,
                .width = 480,
                .init_f = app_init,
                .update_f = app_update,
                .quit_f = app_quit,
        };

        if(!rgl_init(&desc)) {
		return -1;
	}
}

void app_init() {
        srand(time(0));

	bounce_audio_buffer = new rgl_audio_buffer_t;
	rgl_audio_buffer_create_from_vorbis(bounce_audio_buffer, "bounce.ogg");

	init_balls();
	thr_physics = std::thread(thr_physics_func);
}

void app_update(f32 dt) {
	b8 left_pressed = rgl_is_button_pressed(RGL_MOUSE_LEFT);
	b8 right_pressed = rgl_is_button_pressed(RGL_MOUSE_RIGHT);

	v2 mouse_pos;
	rgl_get_cursor_pos(&mouse_pos);

	if(left_pressed || right_pressed) {
		if(selected_ball == NULL) {
			for(ball_t &ball : vec_balls) {
				if(is_point_in_ball(&ball, mouse_pos)) {
					selected_ball = &ball;
					break;
				}
			}
		} else if(left_pressed){
			selected_ball->vel.x = -5.0f * (selected_ball->pos.x - mouse_pos.x);
			selected_ball->vel.y = -5.0f * (selected_ball->pos.y - mouse_pos.y);
		}
	} else {
		if(rgl_is_button_just_released(RGL_MOUSE_RIGHT) && selected_ball != NULL) {
			selected_ball->vel.x = 5.0f * (selected_ball->pos.x - mouse_pos.x);
			selected_ball->vel.y = 5.0f * (selected_ball->pos.y - mouse_pos.y);
		}

		selected_ball = NULL;
	}

	if(rgl_is_key_just_pressed(RGL_KEY_N)) {
		v2 pos;
		rgl_get_cursor_pos(&pos);
		add_ball(pos);

		printf("Adding a new ball at position: [%f, %f]\n", pos.x, pos.y);
	}

	if(rgl_is_key_just_pressed(RGL_KEY_C)) {
		vec_balls.clear();
		printf("Clearing all balls\n");
	}

	if(rgl_is_key_just_pressed(RGL_KEY_G)) {
		gravity = !gravity;
		printf("Gravity: %s\n", gravity ? "ON" : "OFF");
	}

	for(ball_t &ball : vec_balls) {
		/* Apply drag */
		ball.vel.x *= FRICTION;
		ball.vel.y *= FRICTION;

		/* Apply gravity */
		if(gravity) {
			ball.vel.y += GRAVITY * dt;
		}

		ball.pos.x += ball.vel.x * dt;
		ball.pos.y += ball.vel.y * dt;

		if(rgl_v2_len(&ball.vel) < 0.01f) {
			rgl_v2_zero(&ball.vel);
		}
	}

	draw_balls();
}

void app_quit() {
	running = false;
	delete bounce_audio_buffer;
	thr_physics.join();
}

void thr_physics_func() {
	std::vector<collision_t> vec_collisions;

	while(running) {
		u32 i=0; 

		/* Static colliisions */
		for(ball_t &ball : vec_balls) {
			u32 j=0;

			/* Left wall */
			if(ball.pos.x < ball.radius) {
				ball.pos.x = ball.radius; 
				vec_collisions.push_back({ collision_t::COLLISION_W_WALL, &ball, { .pos = {-1, ball.pos.y} } });
			} 
			/* Right wall */
			else if(ball.pos.x + ball.radius > g_rgl_data->width) {
				ball.pos.x = g_rgl_data->width - ball.radius; 
				vec_collisions.push_back({ collision_t::COLLISION_W_WALL, &ball, { .pos = {(f32)g_rgl_data->width+1, ball.pos.y} } });
			} 
			/* Top wall */
			if(ball.pos.y < ball.radius) {
				ball.pos.y = ball.radius; 
				vec_collisions.push_back({ collision_t::COLLISION_W_WALL, &ball, { .pos = {ball.pos.x, -1} } });
			} 
			/* Bottom wall */
			else if(ball.pos.y + ball.radius > g_rgl_data->height) {
				ball.pos.y = g_rgl_data->height - ball.radius; 
				vec_collisions.push_back({ collision_t::COLLISION_W_WALL, &ball, { .pos = {ball.pos.x, (f32)g_rgl_data->height+1} } });
			} 

			/* Overlap collisions */
			for(ball_t &target : vec_balls) {
				/* Ignore checking ball against itself */
				if(i == j++) continue; 

				v2 delta_pos = {
					.x = ball.pos.x - target.pos.x,
					.y = ball.pos.y - target.pos.y,
				};

				/* If balls overlap */
				if(delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y <= (ball.radius + target.radius) * (ball.radius + target.radius)) { 
					vec_collisions.push_back({ collision_t::COLLISION_W_BALL, &ball, { .b = &target } });

					f32 dist = rgl_v2_len(&delta_pos);
					f32 overlap = (dist - ball.radius - target.radius) * 0.5f / dist;

					v2 overlap_move = {
						.x = overlap * delta_pos.x,
						.y = overlap * delta_pos.y,
					};

					rgl_v2_sub(&ball.pos, &overlap_move, &ball.pos);
					rgl_v2_add(&target.pos, &overlap_move, &target.pos);
				}
			}

			++i;
		}

		/* Dynamic collisions */
		for(collision_t &collision : vec_collisions) {
			rgl_audio_source_play(&collision.a->audio_source);

			if(collision.type == collision_t::COLLISION_W_BALL) {
				ball_t *a = collision.a;
				ball_t *b = collision.b;

				v2 delta_pos;
				rgl_v2_sub(&a->pos, &b->pos, &delta_pos);

				v2 delta_vel;
				rgl_v2_sub(&a->vel, &b->vel, &delta_vel);

				f32 dist = rgl_v2_len(&delta_pos);

				v2 normal = {
					.x = (b->pos.x - a->pos.x) / dist,
					.y = (b->pos.y - a->pos.y) / dist,
				};

				f32 p = 2.0f * (normal.x * delta_vel.x + normal.y * delta_vel.y) / (a->radius + b->radius);

				a->vel.x -= p * b->radius * normal.x;
				a->vel.y -= p * b->radius * normal.y;

				b->vel.x += p * a->radius * normal.x;
				b->vel.y += p * a->radius * normal.y;
			} else {
				ball_t *ball = collision.a;

				v2 normal = {
					.x = (collision.pos.x - ball->pos.x) / ball->radius,
					.y = (collision.pos.y - ball->pos.y) / ball->radius,
				};

				if(fabs(normal.x) > 0.1f) ball->vel.x *= -1;
				if(fabs(normal.y) > 0.1f) ball->vel.y *= -1;
			}
		}

		vec_collisions.clear();
	}
}

void draw_balls() {
	for(ball_t &ball : vec_balls) {
		rgl_immediate_draw_circle(ball.color, ball.pos, ball.radius);
	}

	if(selected_ball != NULL) {
		rgl_immediate_draw_circle_outline(RGL_RED, selected_ball->pos, selected_ball->radius, 3.f);

		v2 mouse_pos;
		rgl_get_cursor_pos(&mouse_pos);

		rgl_immediate_draw_line(RGL_BLUE, selected_ball->pos, mouse_pos, 5);
	}
}

void init_balls() {
	v2 pos;
	for(u32 i=0; i<START_BALL_COUNT; ++i) {
		pos.x = (f32)(rand() % g_rgl_data->width);
		pos.y = (f32)(rand() % g_rgl_data->height);

		add_ball(pos);
	}
}

void add_ball(v2 pos) {
	static const f32 HALF_RANDOM_INITIAL_VELOCITY = RANDOM_INITIAL_VELOCITY * 0.5f;

	ball_t ball = {
		.color = { .rgb = { (u8)(RAND_255), (u8)(RAND_255), (u8)(RAND_255) } },
		.radius = (MAX_RADIUS - MIN_RADIUS) * ((f32)rand() / RAND_MAX) + MIN_RADIUS,
	};

	rgl_audio_source_create(&ball.audio_source, bounce_audio_buffer);

	ball.vel.x = rand() % RANDOM_INITIAL_VELOCITY - HALF_RANDOM_INITIAL_VELOCITY;
	ball.vel.y = rand() % RANDOM_INITIAL_VELOCITY - HALF_RANDOM_INITIAL_VELOCITY;

	rgl_v2_cpy(&pos, &ball.pos);

	vec_balls.push_back(ball);
}

b8 is_point_in_ball(ball_t *ball, v2 point) {
	v2 delta_pos = {
		.x = ball->pos.x - point.x,
		.y = ball->pos.y - point.y,
	};
	
	return delta_pos.x*delta_pos.x + delta_pos.y*delta_pos.y < (ball->radius * ball->radius);
}
