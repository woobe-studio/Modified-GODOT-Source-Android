/**************************************************************************/
/*  visual_server_globals.cpp                                             */
/**************************************************************************/


#include "visual_server_globals.h"

RasterizerStorage *VisualServerGlobals::storage = nullptr;
RasterizerCanvas *VisualServerGlobals::canvas_render = nullptr;
RasterizerScene *VisualServerGlobals::scene_render = nullptr;
Rasterizer *VisualServerGlobals::rasterizer = nullptr;

VisualServerCanvas *VisualServerGlobals::canvas = nullptr;
VisualServerViewport *VisualServerGlobals::viewport = nullptr;
VisualServerScene *VisualServerGlobals::scene = nullptr;
