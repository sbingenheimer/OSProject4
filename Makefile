all: libmfs.so server udp.o mkfs.o server.o

libmfs.so: libmfs.o
	gcc -shared -Wl,-soname,libmfs.so -o libmfs.so libmfs.o udp.o -lc	

libmfs.o: libmfs.c
	gcc -fPIC -g -c -Wall libmfs.c udp.c

server.o: server.c
	gcc -fPIC -g -c -Wall server.c udp.c

server: server.o udp.o
	gcc -o server server.o udp.o 

mkfs.o: mkfs.c
	gcc -fPIC -g -c -Wall mkfs.c

clean:
	rm -f server.o udp.o libmfs.so libmfs.o mkfs.o server
	
