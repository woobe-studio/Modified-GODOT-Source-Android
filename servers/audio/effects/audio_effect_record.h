/**************************************************************************/
/*  audio_effect_record.h                                                 */
/**************************************************************************/


#ifndef AUDIO_EFFECT_RECORD_H
#define AUDIO_EFFECT_RECORD_H

#include "core/io/marshalls.h"
#include "core/os/file_access.h"
#include "core/os/os.h"
#include "core/os/thread.h"
#include "editor/import/resource_importer_wav.h"
#include "scene/resources/audio_stream_sample.h"
#include "servers/audio/audio_effect.h"
#include "servers/audio_server.h"

class AudioEffectRecord;

class AudioEffectRecordInstance : public AudioEffectInstance {
	GDCLASS(AudioEffectRecordInstance, AudioEffectInstance);
	friend class AudioEffectRecord;

	bool is_recording;
	Thread io_thread;

	Vector<AudioFrame> ring_buffer;
	Vector<float> recording_data;

	unsigned int ring_buffer_pos;
	unsigned int ring_buffer_mask;
	unsigned int ring_buffer_read_pos;

	void _io_thread_process();
	void _io_store_buffer();
	static void _thread_callback(void *_instance);
	void _update_buffer();
	static void _update(void *userdata);

public:
	void init();
	void finish();
	virtual void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count);
	virtual bool process_silence() const;
};

class AudioEffectRecord : public AudioEffect {
	GDCLASS(AudioEffectRecord, AudioEffect);

	friend class AudioEffectRecordInstance;

	enum {
		IO_BUFFER_SIZE_MS = 1500
	};

	Ref<AudioEffectRecordInstance> current_instance;

	AudioStreamSample::Format format;

	void ensure_thread_stopped();

protected:
	static void _bind_methods();

public:
	Ref<AudioEffectInstance> instance();
	void set_recording_active(bool p_record);
	bool is_recording_active() const;
	void set_format(AudioStreamSample::Format p_format);
	AudioStreamSample::Format get_format() const;
	Ref<AudioStreamSample> get_recording() const;

	AudioEffectRecord();
	~AudioEffectRecord();
};

#endif // AUDIO_EFFECT_RECORD_H
