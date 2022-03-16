#include "rgl_audio.h"
#include "rgl.h"
#include "stb/stb_vorbis.c"
#include <unistd.h>

#define RGL_ASSERT_AUDIO_CONTEXT if(!g_rgl_data->audio_cxt) return

b8 rgl_audio_context_create(rgl_audio_context_t *cxt) {
	RGL_ASSERT_RET_B8(cxt, false);

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
		RGL_LOG_ERROR("Failed to make OpenAL context current"); 
		return false;
	}

	return true;
}

void rgl_audio_context_destroy(rgl_audio_context_t *cxt) {
	RGL_ASSERT(cxt, false);

	alcMakeContextCurrent(0);
	alcDestroyContext(cxt->cxt);

	if(!alcCloseDevice(cxt->dev)) {
		RGL_LOG_ERROR("Failed to close OpenAL device");
	}
}


b8 rgl_audio_buffer_load_from_vorbis(rgl_audio_buffer_t *audio_buffer, const char *path) {
	RGL_ASSERT_AUDIO_CONTEXT false;
	RGL_ASSERT_RET_B8(audio_buffer, false);

	i16 *buffer;
	i32 channels, sample_rate;
	i32 len = stb_vorbis_decode_filename(path, &channels, &sample_rate, &buffer);
	
	if(len <= 0) {
		RGL_LOG_ERROR("Failed to load vorbis file: %s", path);
		return false;
	}

	i32 format = AL_FORMAT_MONO16;
	if(channels == 2) {
		format = AL_FORMAT_STEREO16;
	}

	alGenBuffers(1, &audio_buffer->id);
	alBufferData(audio_buffer->id, format, buffer, len * sizeof(i16), sample_rate);

	free(buffer);

	return true;
}

void rgl_audio_buffer_destroy(rgl_audio_buffer_t *buffer) {
	RGL_ASSERT_AUDIO_CONTEXT;
	RGL_ASSERT(buffer, false);

	if(buffer && buffer->id) {
		alDeleteBuffers(1, &buffer->id);
	}
}

void rgl_audio_source_create(rgl_audio_source_t *source, rgl_audio_buffer_t *buffer) {
	RGL_ASSERT_AUDIO_CONTEXT;
	RGL_ASSERT(buffer, false);
	RGL_ASSERT(source, false);

	source->buffer = buffer;

	alGenSources(1, &source->id);
	alSourcei(source->id, AL_BUFFER, buffer->id);
	alSourcei(source->id, AL_LOOPING, false);
	alSourcei(source->id, AL_POSITION, 0);
	alSourcef(source->id, AL_GAIN, 1.0f);
}

void rgl_audio_source_destroy(rgl_audio_source_t *source) {
	RGL_ASSERT_AUDIO_CONTEXT;
	RGL_ASSERT(source, false);

	if(source && source->id) {
		alDeleteSources(1, &source->id);
	}
}

void rgl_audio_source_play(rgl_audio_source_t *source) {
	RGL_ASSERT_AUDIO_CONTEXT;
	RGL_ASSERT(source, false);

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

void rgl_audio_source_stop(rgl_audio_source_t *source) {
	RGL_ASSERT_AUDIO_CONTEXT;
	RGL_ASSERT(source, false);

	if(source->playing) {
		alSourceStop(source->id);
		source->playing = false;
	}
}

void rgl_audio_source_set_looping(rgl_audio_source_t *source, b8 looping) {
	RGL_ASSERT_AUDIO_CONTEXT;
	RGL_ASSERT(source, false);

	source->looping = looping;
	alSourcei(source->id, AL_LOOPING, looping);
}

void rgl_audio_source_set_gain(rgl_audio_source_t *source, f32 gain) {
	RGL_ASSERT_AUDIO_CONTEXT;
	RGL_ASSERT(source, false);

	alSourcef(source->id, AL_GAIN, gain);
}

void rgl_audio_source_set_pitch(rgl_audio_source_t *source, f32 pitch) {
	RGL_ASSERT_AUDIO_CONTEXT;
	RGL_ASSERT(source, false);

	alSourcef(source->id, AL_PITCH, pitch);
}

b8 rgl_audio_source_is_playing(rgl_audio_source_t *source) {
	RGL_ASSERT_AUDIO_CONTEXT false;
	RGL_ASSERT_RET_B8(source, false);

	i32 state;
	alGetSourcei(source->id, AL_SOURCE_STATE, &state);

	if(state == AL_STOPPED) {
		source->playing = false;
	}

	return source->playing;
}
