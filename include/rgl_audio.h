#ifndef __RGL_AUDIO_H
#define __RGL_AUDIO_H

#include "rgl_common.h"
#include <AL/al.h>
#include <AL/alc.h>

typedef struct rgl_audio_context_t {
	ALCdevice *dev;
	ALCcontext *cxt;
	u32 *audio_effects;
} rgl_audio_context_t;

typedef struct rgl_audio_buffer_t {
	u32 id;
} rgl_audio_buffer_t;

typedef struct rgl_audio_source_t {
	u32 id;
	b8 looping;
	b8 playing;
	rgl_audio_buffer_t *buffer;
} rgl_audio_source_t;

b8 rgl_audio_context_create(rgl_audio_context_t *cxt);
void rgl_audio_context_destroy(rgl_audio_context_t *cxt);

b8 rgl_audio_buffer_load_from_vorbis(rgl_audio_buffer_t *buffer, const char *path);
void rgl_audio_buffer_destroy(rgl_audio_buffer_t *buffer);

void rgl_audio_source_create(rgl_audio_source_t *source, rgl_audio_buffer_t *buffer);
void rgl_audio_source_destroy(rgl_audio_source_t *source);
void rgl_audio_source_play(rgl_audio_source_t *source);
void rgl_audio_source_stop(rgl_audio_source_t *source);
void rgl_audio_source_set_loop(rgl_audio_source_t *source, b8 looping);
void rgl_audio_source_set_gain(rgl_audio_source_t *source, f32 gain);
void rgl_audio_source_set_pitch(rgl_audio_source_t *source, f32 pitch);
b8 rgl_audio_source_is_playing(rgl_audio_source_t *source);

#endif /* __RGL_AUDIO_H */
