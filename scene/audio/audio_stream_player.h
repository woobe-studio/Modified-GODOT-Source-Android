/**************************************************************************/
/*  audio_stream_player.h                                                 */
/**************************************************************************/


#ifndef AUDIO_STREAM_PLAYER_H
#define AUDIO_STREAM_PLAYER_H

#include "core/safe_refcount.h"
#include "scene/main/node.h"
#include "servers/audio/audio_stream.h"

class AudioStreamPlayer : public Node {
	GDCLASS(AudioStreamPlayer, Node);

private:
	Ref<AudioStreamPlayback> stream_playback;
	Ref<AudioStream> stream;
	Vector<AudioFrame> mix_buffer;
	Vector<AudioFrame> fadeout_buffer;
	bool use_fadeout = false;

	SafeNumeric<float> setseek;
	SafeFlag active;
	SafeFlag setstop;
	SafeFlag stop_has_priority;

	float mix_volume_db;
	float volume_db;
	bool autoplay;
	bool stream_paused;
	bool stream_paused_fade;
	StringName bus;

	void _mix_internal(bool p_fadeout);
	void _mix_audio();
	static void _mix_audios(void *self) { reinterpret_cast<AudioStreamPlayer *>(self)->_mix_audio(); }

	void _set_playing(bool p_enable);
	bool _is_active() const;

	void _bus_layout_changed();
	void _mix_to_bus(const AudioFrame *p_frames, int p_amount);

protected:
	void _validate_property(PropertyInfo &property) const;
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_stream(Ref<AudioStream> p_stream);
	Ref<AudioStream> get_stream() const;

	void set_volume_db(float p_volume);
	float get_volume_db() const;

	void play(float p_from_pos = 0.0);
	void seek(float p_seconds);
	void stop();
	bool is_playing() const;
	float get_playback_position();

	void set_bus(const StringName &p_bus);
	StringName get_bus() const;

	void set_autoplay(bool p_enable);
	bool is_autoplay_enabled();

	void set_stream_paused(bool p_pause);
	bool get_stream_paused() const;

	Ref<AudioStreamPlayback> get_stream_playback();

	AudioStreamPlayer();
	~AudioStreamPlayer();
};

#endif // AUDIO_STREAM_PLAYER_H
