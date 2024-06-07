/**************************************************************************/
/*  godot_webgl2.h                                                        */
/**************************************************************************/


#ifndef GODOT_WEBGL2_H
#include "GLES3/gl3.h"

// We could include "webgl/webgl2.h", but old (< 2.0.17) emscripten versions do not have it, so use our own wrapper instead.
void glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);

#endif // GODOT_WEBGL2_H
