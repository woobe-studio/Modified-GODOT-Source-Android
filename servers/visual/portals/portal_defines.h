/**************************************************************************/
/*  portal_defines.h                                                      */
/**************************************************************************/


#ifndef PORTAL_DEFINES_H
#define PORTAL_DEFINES_H

// This file is to allow constants etc to be accessible from outside the visual server,
// while keeping the dependencies to an absolute minimum.

struct PortalDefines {
	static const int OCCLUSION_POLY_MAX_VERTS = 8;
	static const int OCCLUSION_POLY_MAX_HOLES = 4;
};

#endif // PORTAL_DEFINES_H
