/**************************************************************************/
/*  visual_server_globals.h                                               */
/**************************************************************************/


#ifndef VISUAL_SERVER_GLOBALS_H
#define VISUAL_SERVER_GLOBALS_H

#include "rasterizer.h"

class VisualServerCanvas;
class VisualServerViewport;
class VisualServerScene;

class VisualServerGlobals {
public:
	static RasterizerStorage *storage;
	static RasterizerCanvas *canvas_render;
	static RasterizerScene *scene_render;
	static Rasterizer *rasterizer;

	static VisualServerCanvas *canvas;
	static VisualServerViewport *viewport;
	static VisualServerScene *scene;
};

#define VSG VisualServerGlobals

#endif // VISUAL_SERVER_GLOBALS_H
