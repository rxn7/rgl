#include "rgl_audio.h"
#include "rgl.h"
#include "stb/stb_vorbis.c"
#include <unistd.h>

static void _rglInitAudio() {
	if(_rgl_audio_ctx) return;

	_rgl_audio_ctx = malloc(sizeof(rglAudioContext));
	rglAudioContextCreate(_rgl_audio_ctx);
}

void
rglAudioContextCreate(rglAudioContext *cxt) {
	RGL_ASSERT_VALID_PTR(cxt);

	const char *dev_name = alcGetString(0, ALC_DEFAULT_DEVICE_SPECIFIER);
	RGL_LOG("Audio Device: %s", dev_name);

	cxt->dev = alcOpenDevice(dev_name);
	RGL_ASSERT_VALID_PTR(cxt->dev);

	cxt->cxt = alcCreateContext(cxt->dev, 0);
	RGL_ASSERT_VALID_PTR(cxt->cxt);

	RGL_ASSERT(alcMakeContextCurrent(cxt->cxt), "failed to make OpenAL context current");
}

void
rglAudioContextDestroy(rglAudioContext *cxt) {
	RGL_ASSERT_VALID_PTR(cxt);

	alcMakeContextCurrent(0);
	alcDestroyContext(cxt->cxt);

	if(!alcCloseDevice(cxt->dev)) {
		RGL_LOG_ERROR("Failed to close OpenAL device");
	}
}

void
rglAudioBufferLoadFromVorbis(rglAudioBuffer *audio_buffer, const char *path) {
	RGL_ASSERT_VALID_PTR(audio_buffer);
	_rglInitAudio();

	i16 *buffer;
	i32 channels, sample_rate;
	i32 len = stb_vorbis_decode_filename(path, &channels, &sample_rate, &buffer);
	
	RGL_ASSERT(len > 0, "failed to load vorbis file '%s'", path);

	i32 format = AL_FORMAT_MONO16;
	if(channels == 2) {
		format = AL_FORMAT_STEREO16;
	}

	alGenBuffers(1, &audio_buffer->id);
	alBufferData(audio_buffer->id, format, buffer, len * sizeof(i16), sample_rate);

	free(buffer);
}

void
rglAudioBufferDestroy(rglAudioBuffer *buffer) {
	RGL_ASSERT_VALID_PTR(buffer);

	if(buffer && buffer->id) {
		alDeleteBuffers(1, &buffer->id);
	}
}

void
rglAudioSourceCreate(rglAudioSource *source, rglAudioBuffer *buffer) {
	RGL_ASSERT_VALID_PTR(buffer);
	RGL_ASSERT_VALID_PTR(source);
	_rglInitAudio();

	source->buffer = buffer;

	alGenSources(1, &source->id);
	alSourcei(source->id, AL_BUFFER, buffer->id);
	alSourcei(source->id, AL_LOOPING, false);
	alSourcei(source->id, AL_POSITION, 0);
	alSourcef(source->id, AL_GAIN, 1.0f);
}

void
rglAudioSourceDestroy(rglAudioSource *source) {
	RGL_ASSERT_VALID_PTR(source);

	if(source && source->id) {
		alDeleteSources(1, &source->id);
	}
}

void
rglAudioSourcePlay(rglAudioSource *source) {
	RGL_ASSERT_VALID_PTR(source);

	i32 state;
	alGetSourcei(source->id, AL_SOURCE_STATE, &state);
	if(state == AL_STOPPED) {
		source->playing = false;
		alSourcei(source->id, AL_POSITION, 0);
	}

	alSourcei(source->id, AL_POSITION, 0);
	alSourcePlay(source->id);
	source->playing = true;
}

void
rglAudioSourceStop(rglAudioSource *source) {
	RGL_ASSERT_VALID_PTR(source);

	if(source->playing) {
		alSourceStop(source->id);
		source->playing = false;
	}
}

void
rgl_audio_source_set_looping(rglAudioSource *source, b8 looping) {
	RGL_ASSERT_VALID_PTR(source);

	source->looping = looping;
	alSourcei(source->id, AL_LOOPING, looping);
}

void
rglAudioSourceSetGain(rglAudioSource *source, f32 gain) {
	RGL_ASSERT_VALID_PTR(source);

	alSourcef(source->id, AL_GAIN, gain);
}

void
rglAudioSourceSetPitch(rglAudioSource *source, f32 pitch) {
	RGL_ASSERT_VALID_PTR(source);

	alSourcef(source->id, AL_PITCH, pitch);
}

b8
rglAudioSourceIsPlaying(rglAudioSource *source) {
	RGL_ASSERT_VALID_PTR(source);

	i32 state;
	alGetSourcei(source->id, AL_SOURCE_STATE, &state);

	if(state == AL_STOPPED) {
		source->playing = false;
	}

	return source->playing;
}
