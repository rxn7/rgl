#include "rgl_audio.h"
#include "rgl.h"
#include "stb/stb_vorbis.c"
#include <unistd.h>

static ALCdevice *_dev = NULL;
static ALCcontext *_ctx = NULL;

void
rglAudioInit() {
	if(_ctx) return;

	const char *dev_name = alcGetString(0, ALC_DEFAULT_DEVICE_SPECIFIER);
	RGL_LOG("Audio Device: %s", dev_name);

	_dev = alcOpenDevice(dev_name);
	RGL_ASSERT_VALID_PTR(_dev);

	_ctx = alcCreateContext(_dev, 0);
	RGL_ASSERT_VALID_PTR(_ctx);

	RGL_ASSERT(alcMakeContextCurrent(_ctx), "failed to make OpenAL context current");
}

void
rglAudioCleanup() {
	if(!_ctx) return;

	alcMakeContextCurrent(0);
	alcDestroyContext(_ctx);

	if(!alcCloseDevice(_dev)) {
		RGL_LOG_ERROR("Failed to close OpenAL device");
	}
}

void
rglAudioBufferLoadFromVorbis(rglAudioBuffer *audio_buffer, const char *path) {
	RGL_ASSERT_VALID_PTR(audio_buffer);
	rglAudioInit();

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
	rglAudioInit();

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
