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

typedef struct request_struct
{
	int nothin = 0;
} request;

//hold info for thread id, result file, offset, ect...
typedef struct resolve_struct
{
	int nothin = 0;
} resolve;

void* requester_routine(void* request);
void* resolver_routine(void* resolve);

#endif
