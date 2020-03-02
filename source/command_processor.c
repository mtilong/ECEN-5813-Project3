/*
 * command_processor.c
 *
 *  Created on:
 *      Author: 1
 */
#include "command_processor.h"
#include "stdlib.h"
#include "time.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "math.h"
#include "pattern_generator.h"
#include "logger.h"
#include "LED_Control.h"

functionPointerType commands[] = {&allocate,&verify,&patternCommand, &display, &compare, &write, &invert, &freeCommand};



uint8_t* ptr;
uint8_t length;
mem_status allocate(Arguments argc)
{
	length = argc.len;
	ptr = (uint8_t*) malloc(length);
	Log_string("allocated memory\n");
	Log_string("allocate() successful.\n");
	LED_cycleblue_On();
	delay();
	return SUCCESS;
}

mem_status verify(Arguments argc)
{
	if (ptr == NULL)
	{
		LED_cyclered_On();
		Log_string("Memory not allocated.\n");
		return FAILED;
	}
	LED_cyclegreen_On();
	Log_string("Memory verified\n");
	return SUCCESS;
}

mem_status write(Arguments argc)
{
//	*(ptr + argc.start) = argc.writevalue;
	if(argc.len > 2)
	{
		LED_cyclered_On();
		return FAILED;
	}
	if(argc.start+argc.len > length)
	{
		LED_cyclered_On();
		Log_string("write out of range.\n");
		return FAILED;
	}
	uint16_t* ptr2 = (uint16_t*) (ptr + argc.start);
	*ptr2 = argc.writevalue;
	Log_string("write() successful.\n");
	LED_cyclegreen_On();
	return SUCCESS;
}

unsigned long into_binary(long long num)
{
    if(num == 0)
        return 0;
    else
        return (num%2)+10*into_binary(num/2);
}

mem_status display(Arguments argc)
{
	if(argc.len > length)
	{
		Log_string("Display out of range.\n");
		LED_cyclered_On();
		delay();
		return FAILED;
	}
	uint32_t* ptr1 = (uint32_t*)ptr;
	for(int i = 0;i<((argc.len)/4);i++)
	{

	    Log_data(ptr1+i);
	    Log_string(" The Binary is");
	    uint8_t* ptr2 = (uint8_t*) (ptr1+i);
	    for(int i = 0;i<4;i++)
	    {
	    	Log_integer_format(into_binary(*(ptr2+i)));
	    }
	    Log_string("\n");
	}
	LED_cyclegreen_On();
	delay();
	return SUCCESS;
}

mem_status compare(Arguments argc)
{
	uint8_t length = argc.len;
	uint8_t bytearray[length];

	if(argc.len > length)
	{
		Log_string("Compare out of range.\n");
		LED_cyclered_On();
		delay();
		return FAILED;
	}
	for(int i = 0;i<length;i++)
	{
		bytearray[i] = randomnumber(argc.seedvalue,ptr+i);
	}
	for (int i = 0;i<length;i++)
	{
		if (bytearray[i] != *(ptr+i))
		{
			Log_string("compare() FALSE.\n");
			LED_cyclered_On();
			delay();
			return FAILED;
		}
	}
	Log_string("compare TRUE.\n");
	LED_cyclegreen_On();
	delay();
	return SUCCESS;
}

mem_status invert(Arguments argc)
{
	if(argc.len + argc.start > length)
	{
		Log_string("Invert out of range.\n");
		LED_cyclered_On();
		delay();
		return FAILED;
	}
	uint8_t* ptr1 = ptr;
	ptr1 = ptr1 + argc.start;
	for(int i = 0;i < argc.len;i++)
	{
		*(ptr1 + i) = ~(*(ptr1 + i));
	}
	Log_string("invert() successful.\n");
	LED_cyclegreen_On();
	delay();
	return SUCCESS;
}

mem_status freeCommand(Arguments argc)
{
	free(ptr);
	Log_string("freeCommand() successful.\n");
	LED_cyclegreen_On();
	delay();
	return SUCCESS;

}

mem_status patternCommand(Arguments argc)
{
	if(argc.len > length)
	{
		Log_string("Pattern out of range.\n");
		LED_cyclered_On();
		delay();
		return FAILED;
	}
	for(int i = 0;i <argc.len;i++)
	{
		*(ptr + i ) = randomnumber(argc.seedvalue,(ptr+i));
	}
	Log_string("patternCommand() successful.\n");
	LED_cyclegreen_On();
	delay();
	return SUCCESS;
}

void parser(uint16_t* testscript,int arraylength)
{
	//allocate
	Arguments argc;
	if(*(testscript) == 'A')
	{
		argc.len = *(testscript+1);
		commands[0](argc);
		//allocate(argc);
		arraylength -= 2;
		if (arraylength == 0)
			return;
		parser((testscript+2),arraylength);
	}
	//verify
	else if(*(testscript) == 'M')
	{
		commands[1](argc);
		//verify(argc);
		arraylength -= 1;
		if (arraylength == 0)
			return;
		parser((testscript+1),arraylength);
	}
	//pattern
	else if(*(testscript) == 'P')
	{
		argc.len = *(testscript+1);
		argc.seedvalue = *(testscript+2);
		commands[2](argc);
		//pattern(argc);
		arraylength -= 3;
		if(arraylength == 0)
		{
			return;
		}
		parser((testscript+3),arraylength);
	}
	//display
	else if(*(testscript) == 'D')
	{
		argc.len = *(testscript+1);
		commands[3](argc);
		//display(argc);
		arraylength -= 2;
		if(arraylength == 0)
		{
			return;
		}
		parser((testscript+2),arraylength);
	}
	//compare
	else if(*(testscript) == 'C')
	{
		argc.len = *(testscript+1);
		argc.seedvalue = *(testscript+2);
		commands[4](argc);
		//compare(argc);
		arraylength -= 3;
		if(arraylength == 0)
		{
			return;
		}
		parser((testscript+3),arraylength);
	}
	//write
	else if(*(testscript) == 'W')
	{
		argc.len = *(testscript+2);
		argc.start = *(testscript+1);
		argc.writevalue = *(testscript+3);
		commands[5](argc);
		//write(argc);
		arraylength -= 4;
		if(arraylength == 0)
		{
			return;
		}
		parser((testscript+4),arraylength);
	}
	//invert
	else if(*(testscript) == 'I')
	{
		argc.start = *(testscript + 1);
		argc.len = *(testscript + 2);
		commands[6](argc);
		//invert(argc);
		arraylength -= 3;
		if(arraylength == 0)
		{
			return;
		}
		parser((testscript+3),arraylength);
	}
	//free
	else if(*(testscript) == 'F')
	{
		commands[7](argc);
		//freeCommand(argc);
		arraylength -= 1;
		if(arraylength == 0)
		{
			return;
		}
		parser((testscript+1),arraylength);
	}
}

