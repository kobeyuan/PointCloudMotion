#ifndef _GLOBALS_H
#define _GLOBALS_H
#include "basic_types.h"
#include <QMutex>

namespace Thread_Param
{
	extern QMutex sample_set_mutex;
}

namespace Paint_Param
{
	extern ScalarType	g_point_size;
}



#endif