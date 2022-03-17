#ifndef __RGL_AUDIO_H
#define __RGL_AUDIO_H

#include "rgl_common.h"
#include <AL/al.h>
#include <AL/alc.h>

typedef struct rglAudioContext {
	ALCdevice *dev;
	ALCcontext *cxt;
	u32 *audio_effects;
} rglAudioContext;

typedef struct rglAudioBuffer {
	u32 id;
} rglAudioBuffer;

typedef struct rglAudioSource {
	u32 id;
	b8 looping;
	b8 playing;
	rglAudioBuffer *buffer;
} rglAudioSource;

void rgl_audio_context_create(rglAudioContext *cxt);
void rgl_audio_context_destroy(rglAudioContext *cxt);

void rgl_audio_buffer_load_from_vorbis(rglAudioBuffer *buffer, const char *path);
void rgl_audio_buffer_destroy(rglAudioBuffer *buffer);

void rgl_audio_source_create(rglAudioSource *source, rglAudioBuffer *buffer);
void rgl_audio_source_destroy(rglAudioSource *source);
void rgl_audio_source_play(rglAudioSource *source);
void rgl_audio_source_stop(rglAudioSource *source);
void rgl_audio_source_set_loop(rglAudioSource *source, b8 looping);
void rgl_audio_source_set_gain(rglAudioSource *source, f32 gain);
void rgl_audio_source_set_pitch(rglAudioSource *source, f32 pitch);
b8 rgl_audio_source_is_playing(rglAudioSource *source);

#endif /* __RGL_AUDIO_H */
