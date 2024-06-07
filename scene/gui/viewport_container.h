/**************************************************************************/
/*  viewport_container.h                                                  */
/**************************************************************************/


#ifndef VIEWPORT_CONTAINER_H
#define VIEWPORT_CONTAINER_H

#include "scene/gui/container.h"

class ViewportContainer : public Container {
	GDCLASS(ViewportContainer, Container);

	bool stretch;
	int shrink;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_stretch(bool p_enable);
	bool is_stretch_enabled() const;

	void _input(const Ref<InputEvent> &p_event);
	void _unhandled_input(const Ref<InputEvent> &p_event);
	void set_stretch_shrink(int p_shrink);
	int get_stretch_shrink() const;

	virtual Size2 get_minimum_size() const;

	ViewportContainer();
};

#endif // VIEWPORT_CONTAINER_H
