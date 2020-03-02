/*
 * command_processor.h
 *
 *  Created on:
 *      Author: 1
 */
#include "MKL25Z4.h"

typedef struct{
	int len;
	int seedvalue;
	int writevalue;
	int start;
} Arguments;

typedef enum mem_status{
	SUCCESS,
	FAILED,
}mem_status;

typedef mem_status(*functionPointerType)(Arguments) ;

mem_status allocate (Arguments);
mem_status verify (Arguments);
mem_status patternCommand(Arguments);
mem_status display (Arguments);
mem_status compare (Arguments);
mem_status write (Arguments);
mem_status invert (Arguments);
mem_status freeCommand(Arguments);
void parser(uint16_t*,int);
mem_status random(int seedvalue);
