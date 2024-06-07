/**************************************************************************/
/*  audio_effect.h                                                        */
/**************************************************************************/


#ifndef AUDIO_EFFECT_H
#define AUDIO_EFFECT_H

#include "core/math/audio_frame.h"
#include "core/resource.h"

class AudioEffectInstance : public Reference {
	GDCLASS(AudioEffectInstance, Reference);

public:
	virtual void process(const AudioFrame *p_src_frames, AudioFrame *p_dst_frames, int p_frame_count) = 0;
	virtual bool process_silence() const { return false; }
};

class AudioEffect : public Resource {
	GDCLASS(AudioEffect, Resource);

public:
	virtual Ref<AudioEffectInstance> instance() = 0;
	AudioEffect();
};

#endif // AUDIO_EFFECT_H
