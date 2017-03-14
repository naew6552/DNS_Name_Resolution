multi-lookup : multi-lookup.o util.o queue.o
	cc -pthread -o multi-lookup multi-lookup.o util.o queue.o

multi-lookup.o : multi-lookup.c
	cc -c -pthread multi-lookup.c

util.o : util.c
	cc -c util.c

queue.o : queue.c
	cc -c queue.c


clean : 
	rm multi-lookup
