#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "udp.h"

#define BUFFER_SIZE (4096)
int sd;
message_t touse;
sruct stat buff;

// GET CALLS FROM MFS AND DO OPERATIONS THEN RETURN


// UDP read and write
// get the command from the incoming message calls cooresponding method
// in methods, we can update the input message to create a new outgoing message to return
// gets return value from the method (-1, 0) and returns that value to client
void getMSFCommand(){

    //Think we have to make the file image here first then create the socket and wait for messages

    int fd = open(image, O_RDWR);
    if (fd < 0) {
        return -1;
    }

    //getting file information 
    struct stat buff;
    int rd = fstat(fd, &buff);
    if (rd < 0) {
        return -1;
    }

    int image_size = (int) sbuf.st_size;

    void *image = mmap(NULL, image_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    assert(image != MAP_FAILED);


   // so here we wait for messages

    sd = UDP_Open(10000); //not sure what to do here exactly. not sure what the port should be 
    assert(sd > -1);
    while (1) {
	struct sockaddr_in addr;
	char message[BUFFER_SIZE];
	printf("server:: waiting...\n");
	int rc = UDP_Read(sd, &addr, message, BUFFER_SIZE);
	printf("server:: read message [size:%d contents:(%s)]\n", rc, message);

    int fd = open(image, O_RDWR);
    if (fd < 0) {
        return -1;
    }

    //getting file information 
    struct stat buff;
    int rd = fstat(fd, &buff);
    if (rd < 0) {
        return -1;
    }

    int image_size = (int) sbuf.st_size;

    void *image = mmap(NULL, image_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    assert(image != MAP_FAILED);

    touse = (message_t) message;
    int resp;

    if (touse->mtype == MFS_Lookup) {
        resp = lookup(touse, image);
    }
    else if (touse->mtype == MFS_Stat) {
        resp = stat(touse, image);
    }
    else if (touse->mtype == MFS_Write) {
        resp = write(touse, image);
    }
    else if (touse->mtype == MFS_Read) {
        resp = read(touse, image);
    }
    else if (touse->mtype == MFS_Unlink) {
        resp = unlink(touse, image);
    }
    else if (touse->mtype == MFS_Shutdown) {
        resp = shutdown(touse, image);
    }
    else {
        touse->rc = -1;
    }
    
	
    rc = UDP_Write(sd, &addr, (char *) &touse, BUFFER_SIZE);
	printf("server:: reply\n");


    }
    return 0; 

}



int lookup (message_t message, void* image) {

    
    return 0;
}

int stat (message_t message, void* image) {
    return 0;
}

int write (message_t message, void* image) {
    return 0;
}

int read (message_t message, void* image) {
    return 0;
}

int unlink (message_t message, void* image) {
    return 0;
}

int shutdown (message_t message, void* image) {
    return 0;
}

