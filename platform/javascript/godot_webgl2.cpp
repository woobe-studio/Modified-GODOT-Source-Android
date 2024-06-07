/**************************************************************************/
/*  godot_webgl2.cpp                                                      */
/**************************************************************************/


#include "godot_webgl2.h"

extern "C" {
extern void godot_js_display_glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
}

void glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data) {
	godot_js_display_glGetBufferSubData(target, offset, size, data);
}
