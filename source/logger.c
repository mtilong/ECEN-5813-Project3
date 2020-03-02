#include "stdlib.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "logger.h"

uint8_t status;
 void Log_enable()
 {
	 status = 1;
 }
 void Log_disable()
 {
	 status = 0;
 }
uint8_t Log_status()
{
	 return status;
}

void Log_data(uint32_t* ptr)
{
	if (status == 1)
	{
		PRINTF("Memory allocated at 0x%08X Hex is %x ",(ptr),*(ptr));
	}
}

void Log_string(char* s)
{	if(status ==1)
	{
		PRINTF("%s ",s);
	}
}

void Log_integer_format(int d)
{	if(status ==1)
	{
		PRINTF("%08d ",d);
	}
}

