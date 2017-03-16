/***********************332*******************************************************
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
 *	4. Clean up #define's
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
#include <semaphore.h>

#include "util.h"
#include "multi-lookup.h"
#include "queue.h"

#define NUM_THREADS     5
#define MINARGS 3
#define MAX_INPUT_FILES 10
#define MAX_RESOLVER_THREADS 10
#define MIN_RESOLVER_THREADS 2
#define MAX_NAME_LENGTH 1025
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"


//Requester Thread routine, reads from name file and adds names to queue
void* requester_routine(void* input)
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
	
        // Currently, request is going to be used as the threadID
	// This code has been taken from pthread_hello.c as test code.
	request* in = input;
	long* tid = in->id;
	long t;
	char line [MAX_NAME_LENGTH];
	printf("Hello World! It's me, requester_thread #%ld!\n", *tid);
	//long numprint = 3;

	while (fscanf(in->inFile, INPUTFS, line) > 0)
        {
		//printf("%s\n", line);
		while(queue_is_full(in -> q) == 1)
		{
			printf("requester: #%ld is waiting\n", *tid);
			usleep((rand()%100)+1);
		}
		printf("Request Pre sem\n");
		sem_wait(in -> empty);
		sem_wait(in -> mutex);
		/*if(queue_push(in -> q, (void*)line) == QUEUE_FAILURE)
		{
			fprintf(stderr,
        	            	"error: queue_push failed!\n"
                	    	"Value: %s\n"
	                	,line);

		}*/
		queue_push(in -> q, (void*)line);
		printf("%s, added to queue\n", line);
		sem_post(in ->mutex);
		sem_post(in -> full);
		printf("Request post sem\n");
	}
	/*
        * Print hello numprint times *
	for(t=0; t<numprint; t++)
        {
		printf("Hello World! It's me, thread #%ld! "
                   "This is printout %ld of %ld\n",
                   *tid, (t+1), numprint);
                * Sleep for 1 to 2 Seconds *
                usleep((rand()%100)*10000+1000000);
        } */
	

	/* Exit, Returning NULL*/
	return NULL;
}

// Resolver thread routine, will grab name's DNS, and write to result.txt
void* resolver_routine(void* out)
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

        // Currently, request is going to be used as the threadID
	// This code has been taken from pthread_hello.c as test code.
	resolve* output;
	output = out; 
        long* tid = output -> id;
	long t;
        long numprint = 3;
	char* line; // [MAX_NAME_LENGTH];

	/*
        * Print hello numprint times *
        for(t=0; t<numprint; t++)
        {
                printf("Hello World! It's me, thread #%ld! "
                   "This is printout %ld of %ld\n",
                   *tid, (t+1), numprint);
                * Sleep for 1 to 2 Seconds *
                usleep((rand()%100)*10000+1000000);
        }
	*/


	printf("Hello World! It's me, resolver_thread #%ld!\n", *tid);

	while(queue_is_empty(output -> q) == 0)
	{
		printf("Resolver pre sem");
		sem_wait(output -> full);
		sem_wait(output -> mutex);
		/*if((line = queue_pop(output -> q)) == NULL)
		{
			fprintf(stderr, "error: queue_pop failed!\n");
		}
		else
		{
			printf("%s pulled from queue\n", line);
		}*/
		line = queue_pop(output -> q);
		printf("%s pulled from queue\n", line);
		sem_post(output -> mutex);
		sem_post(output -> empty);
		printf("Resolver post sem");
		
	}	

	/*
	char line [MAX_NAME_LENGTH];
	char firstipstr[INET6_ADDRSTRLEN];
	while (fscanf(in->fp, INPUTFS, line) > 0)
	{
		if(dnslookup(line, firstipstr, sizeof(firstipstr)) == UTIL_FAILURE)
		{
                	fprintf(stderr, "dnslookup error: %s\n", line);
                	strncpy(firstipstr, "", sizeof(firstipstr));
           	 }
		printf("%s,%s\n", line, firstipstr);

		
	
		if(queue_is_empty(&q) == 1) {return NULL};
		sem_wait(&mutex);
		queue_push(&q, (void*)
		
	}
	*/

        //pthread_exit(NULL);

        /* Exit, Returning NULL*/
        return NULL;

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

       	/***************************************************************************************
	 * Creating some queue stuff here:
	 **************************************************************************************/
	queue q;
	const int qSize = 10; //this is a test size
	queue_init(&q, qSize); //should be in if for error checking

	/**************************************************************************************
	 * Creating and initializing a semaphore
	 *************************************************************************************/
	sem_t mutex;
	sem_t empty;
	sem_t full;
	sem_init(&mutex, 0, 1); //use if == -1 for error checking
	sem_init(&empty, 0, qSize-1);
	sem_init(&full, 0, qSize-1);
		

	//this stores the threads? 
	pthread_t resolver_threads[NUM_THREADS]; //used to store the identifiers for the threads
	pthread_t requester_threads[argc-2];
	int rc; // used for error checking, stands for return code
	long t; // Keeps track of which thread we're dealing with
	long resolve_cpyt[NUM_THREADS]; //I don't know what this does
	long request_cpyt[argc-2];
	request inputs[argc-2];
	
	/*for(t = 0; t < NUM_THREADS; t++)
	{
		printf("In main: creating resolver_thread %ld\n", t);
		resolve_cpyt[t] = t;
		pthread_create(&(resolver_threads[t]), NULL, resolver_routine, &(resolve_cpyt[t]));
	}*/
	long temp = 1;
	resolve output;
	output.id = &temp;	
	output.outFile = fopen(argv[argc-1], "a+");
	output.q = &q;
	output.mutex = &mutex;
	output.empty = &empty;
	output.full = &full;
	pthread_create(&(resolver_threads[temp]), NULL, resolver_routine, &(output));


	for(t = 0; t < argc-2; t++)
        {
                printf("In main: creating requester_thread %ld\n", t);
                //request_cpyt[t] = t;
		t = (long)t;
		inputs[t].id = &t;	
		inputs[t].inFile = fopen(argv[t+1], "r");
		inputs[t].q = &q;
		inputs[t].mutex = &mutex;
		inputs[t].empty = &empty;
		inputs[t].full = &full;
                pthread_create(&(requester_threads[t]), NULL, requester_routine, &(inputs[t]));
        }
	
	
	printf("In main: creating requester_thread %ld\n", t);
	//request_cpyt[t] = t;
	/*for(t = 0; t < NUM_THREADS; t++)
	{
		pthread_join(resolver_threads[t], NULL);
	}*/

	for(t = 0; t < argc-1; t++)
        {
                pthread_join(requester_threads[t], NULL);
        }
	
	pthread_join(resolver_threads[temp], NULL);

	return 0;
}
