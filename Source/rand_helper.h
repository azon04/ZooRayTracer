#ifndef _RAND_HELPER_H_
#define _RAND_HELPER_H_

#include <stdlib.h>

extern "C"
{
	double drand48();
}

float rand_float()
{
	return drand48();
}


#endif
