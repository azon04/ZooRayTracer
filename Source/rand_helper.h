#ifndef _RAND_HELPER_H_
#define _RAND_HELPER_H_

#include <stdlib.h>

float rand_float()
{
	return rand() / float(RAND_MAX);
}

#endif
