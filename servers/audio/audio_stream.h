/**************************************************************************/
/*  audio_stream.h                                                        */
/**************************************************************************/


#ifndef AUDIO_STREAM_H
#define AUDIO_STREAM_H

#include "core/image.h"
#include "core/resource.h"
#include "servers/audio_server.h"

class AudioStreamPlayback : public Reference {
	GDCLASS(AudioStreamPlayback, Reference);

public:
	virtual void start(float p_from_pos = 0.0) = 0;
	virtual void stop() = 0;
	virtual bool is_playing() const = 0;

	virtual int get_loop_count() const = 0; //times it looped

	virtual float get_playback_position() const = 0;
	virtual void seek(float p_time) = 0;

	virtual void mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames) = 0;
};

class AudioStreamPlaybackResampled : public AudioStreamPlayback {
	GDCLASS(AudioStreamPlaybackResampled, AudioStreamPlayback);

	enum {
		FP_BITS = 16, //fixed point used for resampling
		FP_LEN = (1 << FP_BITS),
		FP_MASK = FP_LEN - 1,
		INTERNAL_BUFFER_LEN = 256,
		CUBIC_INTERP_HISTORY = 4
	};

	AudioFrame internal_buffer[INTERNAL_BUFFER_LEN + CUBIC_INTERP_HISTORY];
	uint64_t mix_offset;

protected:
	void _begin_resample();
	virtual void _mix_internal(AudioFrame *p_buffer, int p_frames) = 0;
	virtual float get_stream_sampling_rate() = 0;

public:
	virtual void mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames);

	AudioStreamPlaybackResampled() { mix_offset = 0; }
};

class AudioStream : public Resource {
	GDCLASS(AudioStream, Resource);
	OBJ_SAVE_TYPE(AudioStream); // Saves derived classes with common type so they can be interchanged.

protected:
	static void _bind_methods();

public:
	virtual Ref<AudioStreamPlayback> instance_playback() = 0;

	virtual float get_length() const = 0; //if supported, otherwise return 0
};

#endif // AUDIO_STREAM_H
