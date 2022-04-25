#ifndef __RGL_AUDIO_H
#define __RGL_AUDIO_H

#include "rgl_common.h"
#include <AL/al.h>
#include <AL/alc.h>

typedef struct rglAudioBuffer {
	u32 id;
} rglAudioBuffer;

typedef struct rglAudioSource {
	u32 id;
	b8 looping;
	b8 playing;
	rglAudioBuffer *buffer;
} rglAudioSource;


void rglAudioInit();
void rglAudioCleanup();

void rglAudioBufferLoadFromVorbis(rglAudioBuffer *buffer, const char *path);
void rglAudioBufferDestroy(rglAudioBuffer *buffer);

void rglAudioSourceCreate(rglAudioSource *source, rglAudioBuffer *buffer);
void rglAudioSourceDestroy(rglAudioSource *source);
void rglAudioSourcePlay(rglAudioSource *source);
void rglAudioSourceStop(rglAudioSource *source);
void rglAudioSourceSetLooping(rglAudioSource *source, b8 looping);
void rglAudioSourceSetGain(rglAudioSource *source, f32 gain);
void rglAudioSourceSetPitch(rglAudioSource *source, f32 pitch);
b8 rglAudioSourceIsPlaying(rglAudioSource *source);

#endif /* __RGL_AUDIO_H */
