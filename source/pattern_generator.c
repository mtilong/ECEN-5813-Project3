#include "command_processor.h"
#include "stdlib.h"
#include "time.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "math.h"
int m;

uint8_t randomnumber(int seedvalue,uint8_t* ptr)
{

	uint8_t address = ptr;
	return fmod(seedvalue*address,256);
}
