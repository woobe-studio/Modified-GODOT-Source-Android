/**************************************************************************/
/*  link_button.h                                                         */
/**************************************************************************/


#ifndef LINK_BUTTON_H
#define LINK_BUTTON_H

#include "scene/gui/base_button.h"
#include "scene/resources/bit_map.h"

class LinkButton : public BaseButton {
	GDCLASS(LinkButton, BaseButton);

public:
	enum UnderlineMode {
		UNDERLINE_MODE_ALWAYS,
		UNDERLINE_MODE_ON_HOVER,
		UNDERLINE_MODE_NEVER
	};

private:
	String text;
	String xl_text;
	UnderlineMode underline_mode;

protected:
	virtual Size2 get_minimum_size() const;
	void _notification(int p_what);
	static void _bind_methods();

public:
	void set_text(const String &p_text);
	String get_text() const;

	void set_underline_mode(UnderlineMode p_underline_mode);
	UnderlineMode get_underline_mode() const;

	LinkButton();
};

VARIANT_ENUM_CAST(LinkButton::UnderlineMode);

#endif // LINK_BUTTON_H
