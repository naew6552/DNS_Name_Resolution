/******************************************************************************
 * FILE: multi-lookup.c
 * DESCRIPTION:
 *   A Pthreaded solution to DNS lookup
 * AUTHOR: Nate Ewan
 *
 * To Do:
 *	1. Geneate pool for resolver threads, using MIN_RESOLVER_THREADS
 *		Have those threads print something.
 *	2. Do the same thing for requester threads
 *	3. Figure out structs for each?
 *
 ******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "util.h"
#include "multi-lookup.h"

#define NUM_THREADS     5
#define MINARGS 3
#define MAX_INPUT_FILES 10
#define MAX_RESOLVER_THREADS 10
#define MIN_RESOLVER_THREADS 2
#define MAX_NAME_LENGTH 1025
#define MAX_IP_LENGTH "INET6 ADDRSTRLEN"
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"


//Requester Thread routine, reads from name file and adds names to queue
void* requester_routine(*void request)
{
       /* Open the file from the filepointer stored in request.
	* 	use while loop for loop throuhg each line of file
	*	check if queue is full, then sleep 0 to 100 microseconds
	* 	
	* 	To do this: P(empty), where empty has the max slots of queue
	* 	then have a second P(mutex) to actually protect the queue from other
	* 	consumers. then queue_push(q, name)
	*	V(mutex), V(full). the V(full) tells the resolver threads
	*	that there is a name in the queue to resolve
	*/
}

// Resolver thread routine, will grab name's DNS, and write to result.txt
void* resolver_routine(*void resolve)
{
       /* first, grab a name from the resuest queue. To do this, full is = 0 initially.
	*	When requester_routine runs, it will V(full) which means there is a name to grab.
	*	
	* So, P(full), then P(mutex). mutex is shared between all routines, and protects the
	*	queue. So once we have P(mutex) we can queue_pop(q). This will get us the 
	* 	next name. Then V(mutex) V(empty). V(empty) tell requester threads that 
	*	there is a new space for a name.
	* 
	* Run the dnslookup(const char* hostname, char* firstIPstr, int maxSize)
	*	command with the name we grabbed, then we have to write to results.txt
	*	This is also a protected resource, but there's only writers/producers,
	*	there are no readers/consumers. So we can use a simple P(mutex2) and
	*	V(mutex2) to protect it.
	*
	*/
}

int main(int argc, char* argv[])
{
	/* Prototyping and thoughts:
	 * 
	 * Question, generate resolvers or requesters first?
	 * 
	 * Generate the requester thread pool with a for loop. if n inputs, generate
	 * 	n total requester threads. Each thread will run requester_routine,
	 *	which will open the name file, and grab the ip name at each line. Then
	 * 	the requester thread will write to the protected queue.
	 * 
	 * Generate the resolver thread pool. Not quite sure how many to use, we'll start
	 * 	with MIN_RESOLVER_THREADS, which is currently 2. Generate using for loop.
	 * 	each resolver thread will run resolver_routine, which will write the 
	 * 	ip name and the resolved DNS ip to a protected file results.txt
	 * 
	 * 		
	 */
}
