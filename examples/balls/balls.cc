/* Settings */
#define RANDOM_INITIAL_VELOCITY 2000
#define MAX_RADIUS 20.f
#define MIN_RADIUS 15.f
#define GRAVITY 980
#define FRICTION 0.99f
#define START_BALL_COUNT 100
#define BOUNCE_AUDIO_SOURCE_COUNT 30
#define BOUNCE_SOUND_PATH "res/bounce.ogg"
#define CLICK_SOUND_PATH "res/click.ogg"

#include <vector> 
#include <thread>
#include <unistd.h>

extern "C" { 
	#include <rgl/rgl.h>
}

struct ball_t {
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
void play_click_sound();
void play_bounce_sound(v2 pos, f32 gain);
void init_balls();
void update_balls();
void draw_balls();
void add_ball(v2 pos);
void thr_physics_func();
b8 is_point_in_ball(ball_t *ball, v2 point);

std::vector<ball_t> vec_balls;
std::thread thr_physics;
ball_t *selected_ball = NULL;

rgl_audio_buffer_t click_audio_buffer;
rgl_audio_source_t click_audio_source;

rgl_audio_buffer_t bounce_audio_buffer;
rgl_audio_source_t bounce_audio_sources[BOUNCE_AUDIO_SOURCE_COUNT];

b8 gravity = false;
b8 muted = false;
b8 running = true;
b8 paused = false;

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

	rgl_audio_buffer_create_from_vorbis(&click_audio_buffer, CLICK_SOUND_PATH);
	rgl_audio_source_create(&click_audio_source, &click_audio_buffer);
	rgl_audio_buffer_create_from_vorbis(&bounce_audio_buffer, BOUNCE_SOUND_PATH);

	for(u32 i=0; i<BOUNCE_AUDIO_SOURCE_COUNT; ++i) {
		rgl_audio_source_create(&bounce_audio_sources[i], &bounce_audio_buffer);
	}

	init_balls();
	thr_physics = std::thread(thr_physics_func);
}

void app_quit() {
	running = false;

	rgl_audio_buffer_destroy(&bounce_audio_buffer);
	rgl_audio_buffer_destroy(&click_audio_buffer);
	rgl_audio_source_destroy(&click_audio_source);

	for(u32 i=0; i<BOUNCE_AUDIO_SOURCE_COUNT; ++i) {
		rgl_audio_source_destroy(&bounce_audio_sources[i]);
	}

	thr_physics.join();
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
			play_click_sound();
			selected_ball->vel.x = 5.0f * (selected_ball->pos.x - mouse_pos.x);
			selected_ball->vel.y = 5.0f * (selected_ball->pos.y - mouse_pos.y);
		}

		selected_ball = NULL;
	}

	if(rgl_is_key_just_pressed(RGL_KEY_N)) {
		v2 pos;
		rgl_get_cursor_pos(&pos);
		add_ball(pos);

		play_click_sound();
		printf("Adding a new ball at position: [%f, %f]\n", pos.x, pos.y);
	}

	if(rgl_is_key_just_pressed(RGL_KEY_C)) {
		vec_balls.clear();
		play_click_sound();
		printf("Clearing all balls\n");
	}

	if(rgl_is_key_just_pressed(RGL_KEY_G)) {
		gravity = !gravity;
		play_click_sound();
		printf("Gravity: %s\n", gravity ? "ON" : "OFF");
	}

	if(rgl_is_key_just_pressed(RGL_KEY_M)) {
		muted = !muted;
		play_click_sound();
		printf("Mute: %s\n", muted ? "ON" : "OFF");
	}

	if(rgl_is_key_just_pressed(RGL_KEY_P)) {
		paused = !paused;
		play_click_sound();
		printf("Paused: %s\n", paused ? "ON" : "OFF");
	}

	f32 radius_multiplier = 1.0f;
	if(rgl_is_key_just_pressed(RGL_KEY_I)) {
		radius_multiplier = 1.1f;
		play_click_sound();
	} else if(rgl_is_key_just_pressed(RGL_KEY_O)) {
		radius_multiplier = 0.9f;
		play_click_sound();
	}

	for(ball_t &ball : vec_balls) {
		/* Apply physics only if game's not paused */
		if(!paused) {
			/* Apply drag */
			ball.vel.x *= FRICTION;
			ball.vel.y *= FRICTION;

			/* Apply gravity */
			if(gravity) {
				ball.vel.y += GRAVITY * dt;
			}

			/* Apply velocity */
			ball.pos.x += ball.vel.x * dt;
			ball.pos.y += ball.vel.y * dt;

			/* Stop balls with very low velocity */
			if(rgl_v2_len(&ball.vel) < 0.01f) {
				rgl_v2_zero(&ball.vel);
			}
		}

		ball.radius *= radius_multiplier;
	}

	draw_balls();
}

void thr_physics_func() {
	std::vector<collision_t> vec_collisions;

	while(running) {
		usleep(1);

		if(paused) continue;

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
			/* Bounce sound */
			f32 speed = rgl_v2_len(&collision.a->vel);
			f32 gain = speed / 1000;
			if(gain > 1.f) gain = 1.f;
			if(speed > 3.f) {
				play_bounce_sound(collision.a->pos, gain);
			}

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

void play_click_sound() {
	puts("Playing click sound");
	rgl_audio_source_set_gain(&click_audio_source, 1.0f);
	rgl_audio_source_set_pitch(&click_audio_source, RAND_RANGE_F(0.8f, 1.2f));
	rgl_audio_source_play(&click_audio_source);
}

void play_bounce_sound(v2 pos, f32 gain) {
	if(muted) return;

	rgl_audio_source_t *source = 0;
	for(u32 i=0; i<BOUNCE_AUDIO_SOURCE_COUNT; ++i) {
		if(!rgl_audio_source_is_playing(&bounce_audio_sources[i])) {
			source = &bounce_audio_sources[i];
			break;
		}
	}

	/* If couldn't find any free audio source, use the first audio source */
	if(!source) {
		source = &bounce_audio_sources[0];
	}

	rgl_audio_source_set_gain(source, gain);
	rgl_audio_source_set_pitch(source, RAND_RANGE_F(0.9f, 1.1f));
	rgl_audio_source_play(source);
}

void add_ball(v2 pos) {
	static const f32 HALF_RANDOM_INITIAL_VELOCITY = RANDOM_INITIAL_VELOCITY * 0.5f;

	ball_t ball = {
		.color = { .rgb = { (u8)(RAND_255), (u8)(RAND_255), (u8)(RAND_255) } },
		.radius = (MAX_RADIUS - MIN_RADIUS) * ((f32)rand() / RAND_MAX) + MIN_RADIUS,
	};

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
