#include "./ball.h"
#include "./collision.h"

#include <vector> 
#include <thread>
#include <unistd.h>

void app_quit(void);
void app_update(f32 dt);
void app_init(void);
void app_draw(void);

void play_click_sound(void);
void play_bounce_sound(f32 gain);
void init_balls(void);
void update_balls(void);
void draw_balls(void);
void add_ball(rglV2 pos);
void thr_physics_func(void);

std::vector<Ball> vec_balls;
std::thread thr_physics;
Ball *selected_ball = NULL;

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
		.height = 720,
		.width = 1280,
		.background_color = RGL_WHITE,
                .init_f = app_init,
                .update_f = app_update,
		.draw_f = app_draw,
                .quit_f = app_quit,
        };

        rglStart(&desc);
}

void
app_init(void) {
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
app_quit(void) {
	running = false;

	rglAudioBufferDestroy(&bounce_audio_buffer);
	rglAudioBufferDestroy(&click_audio_buffer);
	rglAudioSourceDestroy(&click_audio_source);

	for(u32 i=0; i<BOUNCE_AUDIO_SOURCE_COUNT; ++i) {
		rglAudioSourceDestroy(&bounce_audio_sources[i]);
	}

	thr_physics.join();
}

void app_update(f32 dt) {
	b8 left_pressed = rglIsButtonPressed(RGL_MOUSE_LEFT);
	b8 right_pressed = rglIsButtonPressed(RGL_MOUSE_RIGHT);

	rglV2 mouse_pos;
	rglGetCursorPosInWorld(&mouse_pos);

	if(left_pressed || right_pressed) {
		if(selected_ball == NULL) {
			for(Ball &ball : vec_balls) {
				if(ball.isPointInside(mouse_pos)) {
					selected_ball = &ball;
					break;
				}
			}
		} else if(left_pressed){
			selected_ball->velocity.x = -5.0f * (selected_ball->position.x - mouse_pos.x);
			selected_ball->velocity.y = -5.0f * (selected_ball->position.y - mouse_pos.y);
		}
	} else {
		if(rglIsButtonJustReleased(RGL_MOUSE_RIGHT) && selected_ball != NULL) {
			play_click_sound();
			selected_ball->velocity.x = 5.0f * (selected_ball->position.x - mouse_pos.x);
			selected_ball->velocity.y = 5.0f * (selected_ball->position.y - mouse_pos.y);
		}

		selected_ball = NULL;
	}

	if(rglIsKeyJustPressed(RGL_KEY_N)) {
		rglV2 pos;
		rglGetCursorPosInWorld(&pos);
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

	for(Ball &ball : vec_balls) {
		/* Apply physics only if game's not paused */
		if(!paused) {
			/* Apply drag */
			ball.velocity.x *= FRICTION;
			ball.velocity.y *= FRICTION;

			/* Apply gravity */
			if(gravity) {
				ball.velocity.y += GRAVITY * dt;
			}

			/* Apply velocityocity */
			ball.position.x += ball.velocity.x * dt;
			ball.position.y += ball.velocity.y * dt;

			/* Stop balls with very low velocityocity */
			if(rglV2Length(&ball.velocity) < 0.1f) {
				rglV2Zero(&ball.velocity);
			}
		}

		if(selected_ball == NULL || selected_ball == &ball) {
			ball.radius *= radius_multiplier;
		}
	}
}

void app_draw(void) {
	draw_balls();
}

void
thr_physics_func(void) {
	std::vector<Collision> vec_collisions;

	while(running) {
		usleep(1);

		if(paused) continue;

		u32 i=0; 
		/* Static colliisions */
		for(Ball &ball : vec_balls) {
			u32 j=0;

			/* Left wall */
			if(ball.position.x - ball.radius < _rgl_camera->left) {
				ball.position.x = _rgl_camera->left + ball.radius; 
				vec_collisions.push_back({ Collision::COLLISION_W_WALL, &ball, { .position = {_rgl_camera->left, ball.position.y} } });
			} 
			/* Right wall */
			else if(ball.position.x + ball.radius > _rgl_camera->right) {
				ball.position.x = _rgl_camera->right - ball.radius; 
				vec_collisions.push_back({ Collision::COLLISION_W_WALL, &ball, { .position = {_rgl_camera->right, ball.position.y} } });
			} 
			/* Top wall */
			if(ball.position.y + ball.radius > _rgl_camera->bottom) {
				ball.position.y = _rgl_camera->bottom - ball.radius; 
				vec_collisions.push_back({ Collision::COLLISION_W_WALL, &ball, { .position = {ball.position.x, _rgl_camera->bottom } } });
			} 
			/* Bottom wall */
			else if(ball.position.y - ball.radius < _rgl_camera->top) {
				ball.position.y = _rgl_camera->top + ball.radius; 
				vec_collisions.push_back({ Collision::COLLISION_W_WALL, &ball, { .position = {ball.position.x, _rgl_camera->top} } });
			} 

			/* Overlap collisions */
			for(Ball &target : vec_balls) {
				/* Ignore checking ball against itself */
				if(i == j++) continue; 

				rglV2 delta = {
					.x = ball.position.x - target.position.x,
					.y = ball.position.y - target.position.y,
				};

				f32 radius_sum = ball.radius + target.radius;

				/* If balls overlap */
				if(delta.x*delta.x + delta.y*delta.y <= radius_sum*radius_sum) { 
					vec_collisions.push_back({ Collision::COLLISION_W_BALL, &ball, { .b = &target } });

					f32 dist = rglV2Length(&delta);
					f32 overlap = (dist - ball.radius - target.radius) * 0.5f / dist;

					rglV2 overlap_move = {
						.x = overlap * delta.x,
						.y = overlap * delta.y,
					};

					rglV2Sub(&ball.position, &overlap_move, &ball.position);
					rglV2Add(&target.position, &overlap_move, &target.position);
				}
			}

			++i;
		}

		/* Dynamic collisions */
		for(Collision &collision : vec_collisions) {
			f32 time = rglGetTime();
			if(collision.type == Collision::COLLISION_W_BALL) {
				Ball *a = collision.a;
				Ball *b = collision.b;

				rglV2 delta_pos;
				rglV2Sub(&a->position, &b->position, &delta_pos);

				rglV2 delta_vel;
				rglV2Sub(&a->velocity, &b->velocity, &delta_vel);

				f32 dist = rglV2Length(&delta_pos);

				rglV2 normal = {
					.x = (b->position.x - a->position.x) / dist,
					.y = (b->position.y - a->position.y) / dist,
				};

				f32 force = 2.0f * (normal.x * delta_vel.x + normal.y * delta_vel.y) / (a->radius + b->radius);

				/* Bounce sound */
				if(time - a->last_sound_played_time >= MIN_TIME_BETWEEN_BOUNCE_SOUNDS && time - b->last_sound_played_time >= MIN_TIME_BETWEEN_BOUNCE_SOUNDS) {
					a->last_sound_played_time = time;
					b->last_sound_played_time = time;

					f32 gain = force / 100;
					if(gain > 1.f) gain = 1.f;
					play_bounce_sound(gain);
				}

				a->velocity.x -= force * b->radius * normal.x;
				a->velocity.y -= force * b->radius * normal.y;

				b->velocity.x += force * a->radius * normal.x;
				b->velocity.y += force * a->radius * normal.y;
			} else {
				Ball *ball = collision.a;

				rglV2 normal = {
					.x = (collision.position.x - ball->position.x) / ball->radius,
					.y = (collision.position.y - ball->position.y) / ball->radius,
				};

				/* Bounce sound */
				if(time - collision.a->last_sound_played_time >= MIN_TIME_BETWEEN_BOUNCE_SOUNDS) {
					f32 speed = rglV2Length(&collision.a->velocity);

					if(speed > 15.f) {
						collision.a->last_sound_played_time = time;
						f32 gain = speed / 5000;
						if(gain > 1.f) gain = 1.f;
						play_bounce_sound(gain);
					}
				}

				if(fabs(normal.x) > 0.1f) ball->velocity.x *= -1;
				if(fabs(normal.y) > 0.1f) ball->velocity.y *= -1;
			}
		}

		vec_collisions.clear();
	}
}

void
draw_balls(void) {
	for(Ball &ball : vec_balls) {
		rglDrawCircle(ball.color, ball.position, ball.radius);
	}

	if(selected_ball != NULL) {
		rglDrawCircleOutline(RGL_RED, selected_ball->position, selected_ball->radius, 3.f);

		rglV2 mouse_pos;
		rglGetCursorPosInWorld(&mouse_pos);

		rglDrawLine(RGL_BLUE, selected_ball->position, mouse_pos, 5);
	}
}

void
init_balls(void) {
	rglV2 pos;
	for(u32 i=0; i<START_BALL_COUNT; ++i) {
		pos.x = (f32)(rand() % _rgl_width);
		pos.y = (f32)(rand() % _rgl_height);

		add_ball(pos);
	}
}

void
play_click_sound(void) {
	rglAudioSourceSetGain(&click_audio_source, 1.0f);
	rglAudioSourceSetPitch(&click_audio_source, RGL_RAND_RANGE_F32(0.8f, 1.2f));
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
	rglAudioSourceSetPitch(source, RGL_RAND_RANGE_F32(0.9f, 1.1f));
	rglAudioSourcePlay(source);
}

void
add_ball(rglV2 pos) {
	static const f32 HALF_RANDOM_INIT_VEL = RANDOM_INIT_VEL * 0.5f;

	Ball ball(pos);
	ball.velocity = { rand() % RANDOM_INIT_VEL - HALF_RANDOM_INIT_VEL, rand() % RANDOM_INIT_VEL - HALF_RANDOM_INIT_VEL };

	vec_balls.push_back(ball);
}
