#include "rgl_audio.h"
#include "stb/stb_vorbis.c"
#include <unistd.h>

b8 rgl_audio_context_create(rgl_audio_context_t *cxt) {
#ifdef RGL_DEBUG
	if(!cxt) {
		RGL_LOG_ERROR("Can't create audio context with null pointer");
		return false;
	}
#endif

	const char *dev_name = alcGetString(0, ALC_DEFAULT_DEVICE_SPECIFIER);
	RGL_LOG("Audio Device: %s", dev_name);

	cxt->dev = alcOpenDevice(dev_name);
	if(!cxt->dev) {
		RGL_LOG_ERROR("Failed to open OpenAL device");
		return false;
	}

	cxt->cxt = alcCreateContext(cxt->dev, 0);
	if(!cxt->cxt) {
		RGL_LOG_ERROR("Failed to create OpenAL context");
		return false;
	}

	if(!alcMakeContextCurrent(cxt->cxt)) {
		RGL_LOG_ERROR("Failed to make OpenAL context current"); return false;
	}

	return true;
}

b8 rgl_audio_context_destroy(rgl_audio_context_t *cxt) {
#ifdef RGL_DEBUG
	if(!cxt) {
		RGL_LOG_ERROR("Can't destroy audio context with null pointer");
		return false;
	}
#endif
	alcMakeContextCurrent(0);

	alcDestroyContext(cxt->cxt);

	if(!alcCloseDevice(cxt->dev)) {
		RGL_LOG_ERROR("Failed to close OpenAL device");
		return false;
	}

	return true;
}


void rgl_audio_buffer_create_from_vorbis(rgl_audio_buffer_t *audio_buffer, const char *path) {
	s16 *buffer;
	s32 channels, sample_rate;
	s32 len = stb_vorbis_decode_filename(path, &channels, &sample_rate, &buffer);

	s32 format = AL_FORMAT_MONO16;
	if(channels == 2) {
		format = AL_FORMAT_STEREO16;
	}

	alGenBuffers(1, &audio_buffer->id);
	alBufferData(audio_buffer->id, format, buffer, len * sizeof(s16), sample_rate);

	free(buffer);
}

void rgl_audio_buffer_destroy(rgl_audio_buffer_t *buffer) {
	if(buffer && buffer->id) {
		alDeleteBuffers(1, &buffer->id);
	}
}

void rgl_audio_source_create(rgl_audio_source_t *source, rgl_audio_buffer_t *buffer) {
	source->buffer = buffer;

	alGenSources(1, &source->id);
	alSourcei(source->id, AL_BUFFER, buffer->id);
	alSourcei(source->id, AL_LOOPING, false);
	alSourcei(source->id, AL_POSITION, 0);
	alSourcef(source->id, AL_GAIN, 1.0f);
}

void rgl_audio_source_destroy(rgl_audio_source_t *source) {
	if(source && source->id) {
		alDeleteSources(1, &source->id);
	}
}

void rgl_audio_source_play(rgl_audio_source_t *source) {
	s32 state;
	alGetSourcei(source->id, AL_SOURCE_STATE, &state);
	if(state == AL_STOPPED) {
		source->playing = false;
		alSourcei(source->id, AL_POSITION, 0);
	}

	alSourcei(source->id, AL_POSITION, 0);
	alSourcePlay(source->id);
	source->playing = true;
}

void rgl_audio_source_stop(rgl_audio_source_t *source) {
	if(source->playing) {
		alSourceStop(source->id);
		source->playing = false;
	}
}

void rgl_audio_source_set_looping(rgl_audio_source_t *source, b8 looping) {
	source->looping = looping;
	alSourcei(source->id, AL_LOOPING, looping);
}

void rgl_audio_source_set_gain(rgl_audio_source_t *source, f32 gain) {
	alSourcef(source->id, AL_GAIN, gain);
}

void rgl_audio_source_set_pitch(rgl_audio_source_t *source, f32 pitch) {
	alSourcef(source->id, AL_PITCH, pitch);
}

b8 rgl_audio_source_is_playing(rgl_audio_source_t *source) {
	s32 state;
	alGetSourcei(source->id, AL_SOURCE_STATE, &state);

	if(state == AL_STOPPED) {
		source->playing = false;
	}

	return source->playing;
}
