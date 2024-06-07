/**************************************************************************/
/*  nav_rid.h                                                             */
/**************************************************************************/


#ifndef NAV_RID_H
#define NAV_RID_H

#include "core/rid.h"

class NavRid : public RID_Data {
	RID self;

public:
	_FORCE_INLINE_ void set_self(const RID &p_self) { self = p_self; }
	_FORCE_INLINE_ RID get_self() const { return self; }
};

#endif // NAV_RID_H
