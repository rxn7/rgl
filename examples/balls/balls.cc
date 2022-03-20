/* Settings */
#define RANDOM_INITIAL_VELOCITY 2000
#define MAX_RADIUS 20.f
#define MIN_RADIUS 15.f
#define GRAVITY 980
#define FRICTION 0.99f
#define START_BALL_COUNT 100
#define MIN_TIME_BETWEEN_BOUNCE_SOUNDS 0.1f
#define BOUNCE_AUDIO_SOURCE_COUNT 30
#define CLICK_SOUND_PATH "res/click.ogg"
#define BOUNCE_SOUND_PATH "res/bounce.ogg"

#include <vector> 
#include <thread>
#include <unistd.h>

extern "C" { 
	#include <rgl/rgl.h>
}

struct ball_t {
	rglColor color;
	v2 pos;
	v2 vel;
	f32 radius;
	f32 sound_played_time;
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
void play_bounce_sound(f32 gain);
void init_balls();
void update_balls();
void draw_balls();
void add_ball(v2 pos);
void thr_physics_func();
b8 is_point_in_ball(ball_t *ball, v2 point);

std::vector<ball_t> vec_balls;
std::thread thr_physics;
ball_t *selected_ball = NULL;

rglAudioBuffer click_audio_buffer;
rglAudioSource click_audio_source;

rglAudioBuffer bounce_audio_buffer;
rglAudioSource bounce_audio_sources[BOUNCE_AUDIO_SOURCE_COUNT];

b8 gravity = false;
b8 muted = false;
b8 running = true;
b8 paused = false;

int
main(int argc, const char **argv) {
        rglAppDesc desc = {
                .title = "RGL | Balls",
                .height = 480,
                .width = 640,
                .init_f = app_init,
                .update_f = app_update,
                .quit_f = app_quit,
        };

        rglStart(&desc);
}

void
app_init() {
        srand(time(0));

	rglAudioBufferLoadFromVorbis(&click_audio_buffer, CLICK_SOUND_PATH);
	rglAudioSourceCreate(&click_audio_source, &click_audio_buffer);
	rglAudioBufferLoadFromVorbis(&bounce_audio_buffer, BOUNCE_SOUND_PATH);

	for(u32 i=0; i<BOUNCE_AUDIO_SOURCE_COUNT; ++i) {
		rglAudioSourceCreate(&bounce_audio_sources[i], &bounce_audio_buffer);
	}

	init_balls();
	thr_physics = std::thread(thr_physics_func);
}

void
app_quit() {
	running = false;

	rglAudioBufferDestroy(&bounce_audio_buffer);
	rglAudioBufferDestroy(&click_audio_buffer);
	rglAudioSourceDestroy(&click_audio_source);

	for(u32 i=0; i<BOUNCE_AUDIO_SOURCE_COUNT; ++i) {
		rglAudioSourceDestroy(&bounce_audio_sources[i]);
	}

	thr_physics.join();
}

void
app_update(f32 dt) {
	b8 left_pressed = rglIsButtonPressed(RGL_MOUSE_LEFT);
	b8 right_pressed = rglIsButtonPressed(RGL_MOUSE_RIGHT);

	v2 mouse_pos;
	rglGetCursorPos(&mouse_pos);

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
		if(rglIsButtonJustReleased(RGL_MOUSE_RIGHT) && selected_ball != NULL) {
			play_click_sound();
			selected_ball->vel.x = 5.0f * (selected_ball->pos.x - mouse_pos.x);
			selected_ball->vel.y = 5.0f * (selected_ball->pos.y - mouse_pos.y);
		}

		selected_ball = NULL;
	}

	if(rglIsKeyJustPressed(RGL_KEY_N)) {
		v2 pos;
		rglGetCursorPos(&pos);
		add_ball(pos);

		play_click_sound();
		printf("Adding a new ball at position: [%f, %f]\n", pos.x, pos.y);
	}

	if(rglIsKeyJustPressed(RGL_KEY_C)) {
		vec_balls.clear();
		play_click_sound();
		printf("Clearing all balls\n");
	}

	if(rglIsKeyJustPressed(RGL_KEY_G)) {
		gravity = !gravity;
		play_click_sound();
		printf("Gravity: %s\n", gravity ? "ON" : "OFF");
	}

	if(rglIsKeyJustPressed(RGL_KEY_M)) {
		muted = !muted;
		play_click_sound();
		printf("Mute: %s\n", muted ? "ON" : "OFF");
	}

	if(rglIsKeyJustPressed(RGL_KEY_P)) {
		paused = !paused;
		play_click_sound();
		printf("Paused: %s\n", paused ? "ON" : "OFF");
	}

	f32 radius_multiplier = 1.0f;
	if(rglIsKeyJustPressed(RGL_KEY_I)) {
		radius_multiplier = 1.1f;
		play_click_sound();
	} else if(rglIsKeyJustPressed(RGL_KEY_O)) {
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
			if(rglV2Length(&ball.vel) < 0.1f) {
				rglV2Zero(&ball.vel);
			}
		}

		if(selected_ball == NULL || selected_ball == &ball) {
			ball.radius *= radius_multiplier;
		}
	}

	draw_balls();
}

void
thr_physics_func() {
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
			else if(ball.pos.x + ball.radius > _rgl_data->width) {
				ball.pos.x = _rgl_data->width - ball.radius; 
				vec_collisions.push_back({ collision_t::COLLISION_W_WALL, &ball, { .pos = {(f32)_rgl_data->width+1, ball.pos.y} } });
			} 
			/* Top wall */
			if(ball.pos.y < ball.radius) {
				ball.pos.y = ball.radius; 
				vec_collisions.push_back({ collision_t::COLLISION_W_WALL, &ball, { .pos = {ball.pos.x, -1} } });
			} 
			/* Bottom wall */
			else if(ball.pos.y + ball.radius > _rgl_data->height) {
				ball.pos.y = _rgl_data->height - ball.radius; 
				vec_collisions.push_back({ collision_t::COLLISION_W_WALL, &ball, { .pos = {ball.pos.x, (f32)_rgl_data->height+1} } });
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

					f32 dist = rglV2Length(&delta_pos);
					f32 overlap = (dist - ball.radius - target.radius) * 0.5f / dist;

					v2 overlap_move = {
						.x = overlap * delta_pos.x,
						.y = overlap * delta_pos.y,
					};

					rglV2Sub(&ball.pos, &overlap_move, &ball.pos);
					rglV2Add(&target.pos, &overlap_move, &target.pos);
				}
			}

			++i;
		}

		/* Dynamic collisions */
		for(collision_t &collision : vec_collisions) {
			f32 time = rglGetTime();
			if(collision.type == collision_t::COLLISION_W_BALL) {
				ball_t *a = collision.a;
				ball_t *b = collision.b;

				v2 delta_pos;
				rglV2Sub(&a->pos, &b->pos, &delta_pos);

				v2 delta_vel;
				rglV2Sub(&a->vel, &b->vel, &delta_vel);

				f32 dist = rglV2Length(&delta_pos);

				v2 normal = {
					.x = (b->pos.x - a->pos.x) / dist,
					.y = (b->pos.y - a->pos.y) / dist,
				};

				f32 force = 2.0f * (normal.x * delta_vel.x + normal.y * delta_vel.y) / (a->radius + b->radius);

				/* Bounce sound */
				if(time - a->sound_played_time >= MIN_TIME_BETWEEN_BOUNCE_SOUNDS && time - b->sound_played_time >= MIN_TIME_BETWEEN_BOUNCE_SOUNDS) {
					a->sound_played_time = time;
					b->sound_played_time = time;

					f32 gain = force / 100;
					if(gain > 1.f) gain = 1.f;
					play_bounce_sound(gain);
				}

				a->vel.x -= force * b->radius * normal.x;
				a->vel.y -= force * b->radius * normal.y;

				b->vel.x += force * a->radius * normal.x;
				b->vel.y += force * a->radius * normal.y;
			} else {
				ball_t *ball = collision.a;

				v2 normal = {
					.x = (collision.pos.x - ball->pos.x) / ball->radius,
					.y = (collision.pos.y - ball->pos.y) / ball->radius,
				};

				/* Bounce sound */
				if(time - collision.a->sound_played_time >= MIN_TIME_BETWEEN_BOUNCE_SOUNDS) {
					f32 speed = rglV2Length(&collision.a->vel);

					if(speed > 15.f) {
						collision.a->sound_played_time = time;
						f32 gain = speed / 5000;
						if(gain > 1.f) gain = 1.f;
						play_bounce_sound(gain);
					}
				}

				if(fabs(normal.x) > 0.1f) ball->vel.x *= -1;
				if(fabs(normal.y) > 0.1f) ball->vel.y *= -1;
			}
		}

		vec_collisions.clear();
	}
}

void
draw_balls() {
	for(ball_t &ball : vec_balls) {
		rglDrawCircle(ball.color, ball.pos, ball.radius);
	}

	if(selected_ball != NULL) {
		rglDrawCircleOutline(RGL_RED, selected_ball->pos, selected_ball->radius, 3.f);

		v2 mouse_pos;
		rglGetCursorPos(&mouse_pos);

		rglDrawLine(RGL_BLUE, selected_ball->pos, mouse_pos, 5);
	}
}

void
init_balls() {
	v2 pos;
	for(u32 i=0; i<START_BALL_COUNT; ++i) {
		pos.x = (f32)(rand() % _rgl_data->width);
		pos.y = (f32)(rand() % _rgl_data->height);

		add_ball(pos);
	}
}

void
play_click_sound() {
	rglAudioSourceSetGain(&click_audio_source, 1.0f);
	rglAudioSourceSetPitch(&click_audio_source, RAND_RANGE_F(0.8f, 1.2f));
	rglAudioSourcePlay(&click_audio_source);
}

void
play_bounce_sound(f32 gain) {
	if(muted) return;

	rglAudioSource *source = 0;
	for(u32 i=0; i<BOUNCE_AUDIO_SOURCE_COUNT; ++i) {
		if(!rglAudioSourceIsPlaying(&bounce_audio_sources[i])) {
			source = &bounce_audio_sources[i];
			break;
		}
	}

	/* If couldn't find any free audio source, use the first audio source */
	if(!source) {
		source = &bounce_audio_sources[0];
	}

	rglAudioSourceSetGain(source, gain);
	rglAudioSourceSetPitch(source, RAND_RANGE_F(0.9f, 1.1f));
	rglAudioSourcePlay(source);
}

void
add_ball(v2 pos) {
	static const f32 HALF_RANDOM_INITIAL_VELOCITY = RANDOM_INITIAL_VELOCITY * 0.5f;

	ball_t ball = {
		.color = { .rgb = { (u8)(RAND_255), (u8)(RAND_255), (u8)(RAND_255) } },
		.radius = (MAX_RADIUS - MIN_RADIUS) * ((f32)rand() / RAND_MAX) + MIN_RADIUS,
	};

	ball.vel.x = rand() % RANDOM_INITIAL_VELOCITY - HALF_RANDOM_INITIAL_VELOCITY;
	ball.vel.y = rand() % RANDOM_INITIAL_VELOCITY - HALF_RANDOM_INITIAL_VELOCITY;

	rglV2Copy(&pos, &ball.pos);

	vec_balls.push_back(ball);
}

b8
is_point_in_ball(ball_t *ball, v2 point) {
	v2 delta_pos = {
		.x = ball->pos.x - point.x,
		.y = ball->pos.y - point.y,
	};
	
	return delta_pos.x*delta_pos.x + delta_pos.y*delta_pos.y < (ball->radius * ball->radius);
}
