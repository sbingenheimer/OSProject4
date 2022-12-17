#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "udp.h"
#include "message.h"

#define BUFFER_SIZE (4096)
int sd;
message_t touse;
struct stat buff;
char * port;
char * imageIn;

// GET CALLS FROM MFS AND DO OPERATIONS THEN RETURN



int serverLookup (message_t message, void* image) {
    
    
    return 0;
}

int serverStat (message_t message, void* image) {
    return 0;
}

int serverWrite (message_t message, void* image) {
    return 0;
}

int serverRead (message_t message, void* image) {
    return 0;
}

int serverUnlink (message_t message, void* image) {
    return 0;
}

int serverShutdown (message_t message, void* image) {
    return 0;
}

// UDP read and write
// get the command from the incoming message calls cooresponding method
// in methods, we can update the input message to create a new outgoing message to return
// gets return value from the method (-1, 0) and returns that value to client
int main(int argc, char *argv[]) {

    //Think we have to make the file image here first then create the socket and wait for messages

   
    port = argv[1];
    int portInt = atoi(port);

    imageIn = argv[2];


    int fd = open(imageIn, O_RDWR);
    if (fd < 0) {
        return -1;
    }

    //getting file information 
    struct stat buff;
    int rd = fstat(fd, &buff);
    if (rd < 0) {
        return -1;
    }

    int image_size = (int) buff.st_size;

    void *image = mmap(NULL, image_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (image == MAP_FAILED) {
        printf("image does not exist\n");
    }


   // so here we wait for messages

    sd = UDP_Open(portInt); //not sure what to do here exactly. not sure what the port should be 
    //assert(sd > -1);
    while (1) {
	struct sockaddr_in addr;
	printf("server:: waiting...\n");
	int rc = UDP_Read(sd, &addr, (char*)&touse, BUFFER_SIZE);
	printf("server:: read message [size:%d contents]\n", rc);

   
    int resp;

    if (strcmp(touse.mtype, "MFS_Lookup")) {
        resp = serverLookup(touse, image);
        touse.rc = resp;
    }
    else if (strcmp(touse.mtype, "MFS_Stat")) {
        resp = serverStat(touse, image);
        touse.rc = resp;
    }
    else if (strcmp(touse.mtype, "MFS_Write")) {
        resp = serverWrite(touse, image);
        touse.rc = resp;
    }
    else if (strcmp(touse.mtype, "MFS_Read")) {
        resp = serverRead(touse, image);
        touse.rc = resp;
    }
    else if (strcmp(touse.mtype, "MFS_Unlink")) {
        resp = serverUnlink(touse, image);
        touse.rc = resp;
    }
    else if (strcmp(touse.mtype, "MFS_Shutdown")) {
        resp = serverShutdown(touse, image);
        touse.rc = resp;
    }
    else {
        touse.rc = -1;
    }
    
	
    rc = UDP_Write(sd, &addr, (char *) &touse, BUFFER_SIZE);
	printf("server:: reply\n");


    }

    close(fd);
    return 0; 
    
}
