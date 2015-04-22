CC = g++

all: DMcache SAcache

DMcache: DMcache.o
	$(CC) -o DMcache DMcache.o
	
DMcache.o: DMCache.cpp
	$(CC) -o DMcache.o -c DMCache.cpp

SAcache: SAcache.o
	$(CC) -o SAcache SAcache.o

SAcache.o: SACache.cpp
	$(CC) -o SAcache.o -c SACache.cpp
	
clean:
	rm DMcache DMcache.o SAcache SAcache.o
