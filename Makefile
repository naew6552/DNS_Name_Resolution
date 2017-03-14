multi-lookup : multi-lookup.o util.o
	cc -pthread -o multi-lookup multi-lookup.o util.o

multi-lookup.o : multi-lookup.c
	cc -c -pthread multi-lookup.c

util.o : util.c
	cc -c util.c

clean : 
	rm multi-lookup
