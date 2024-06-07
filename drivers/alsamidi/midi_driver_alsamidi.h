/**************************************************************************/
/*  midi_driver_alsamidi.h                                                */
/**************************************************************************/


#ifndef MIDI_DRIVER_ALSAMIDI_H
#define MIDI_DRIVER_ALSAMIDI_H

#ifdef ALSAMIDI_ENABLED

#include "core/os/midi_driver.h"
#include "core/os/mutex.h"
#include "core/os/thread.h"
#include "core/safe_refcount.h"
#include "core/vector.h"

#include "../alsa/asound-so_wrap.h"
#include <stdio.h>

class MIDIDriverALSAMidi : public MIDIDriver {
	Thread thread;
	Mutex mutex;

	Vector<snd_rawmidi_t *> connected_inputs;

	SafeFlag exit_thread;

	static void thread_func(void *p_udata);

	void lock() const;
	void unlock() const;

public:
	virtual Error open();
	virtual void close();

	virtual PoolStringArray get_connected_inputs();

	MIDIDriverALSAMidi();
	virtual ~MIDIDriverALSAMidi();
};

#endif // ALSAMIDI_ENABLED

#endif // MIDI_DRIVER_ALSAMIDI_H
