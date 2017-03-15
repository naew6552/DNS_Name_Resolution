/*
 * FILE: multi-lookup.h
 * Author: Nate Ewan 
 * Project: CSCI 3753 Programming Assignment 3
 * Create Date: 2017/03/12
 * Description:
 *      This is the header file for a multi-threaded DNS Lookup Resolver
 */

#ifndef MULTILOOKUP_H
#define MULTILOOKUP_H

#include "queue.h"
#include <semaphore.h>

typedef struct request_struct
{
	FILE *inFile;
	long* id;
	queue* q;
	sem_t *binary;
	
} request;

//hold info for thread id, result file, offset, ect...
typedef struct resolve_struct
{
	FILE *outFile;
	long* id;
	queue* q;
	sem_t *binary;
} resolve;

void* requester_routine(void* input);
void* resolver_routine(void* out);

#endif
