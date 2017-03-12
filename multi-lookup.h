i/*
 * FILE: multi-lookup.h
 * Author: Nate Ewan 
 * Project: CSCI 3753 Programming Assignment 3
 * Create Date: 2017/03/12
 * Description:
 *      This is the header file for a multi-threaded DNS Lookup Resolver
 */

#ifndef MULTI-LOOKUP_H
#define MULTI-LOOKUP_H

typedef request_struct()
{

} request;

//hold info for thread id, result file, offset, ect...
typedef resolve_struct()
{

} resolve;

void* requester_routine(*void request);
void* resolver_routine(*void resolve);

#endif
