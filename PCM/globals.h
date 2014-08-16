#ifndef _GLOBALS_H
#define _GLOBALS_H
#include "basic_types.h"
#include <QMutex>


namespace Paint_Param
{
	extern ScalarType	g_point_size;
}

#define		LOCK(x)		x->lock()
#define		UNLOCK(x)	x->unlock()

#endif