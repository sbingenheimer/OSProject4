#include <stdio.h>
#include "mfs.h"

#define BUFFER_SIZE (4096);
int sd;
int fc;
struct sockaddr_in addrSnd, addrRcv;


//should prob make a send function so we dont have to rewrite every method when sending to and from client and server
//
//

int MFS_Init(char *hostname, int port) {
    printf("MFS Init2 %s %d\n", hostname, port);
    
    //Think this is how you initialize???
    sd = UDP_Open(0);
    if (sd < 0) {
        return -1;
    }
    fc = UDP_FillSockAddr(addrSnd, hostname, port);
    if (fc < 0){
        return -1;
    }else {
       return 0;
    }
}

int MFS_Lookup(int pinum, char *name) {
    // network communication to do the lookup to server

    return 0;
}

int MFS_Stat(int inum, MFS_Stat_t *m) {
    return 0;
}

int MFS_Write(int inum, char *buffer, int offset, int nbytes) {
    return 0;
}

int MFS_Read(int inum, char *buffer, int offset, int nbytes) {
    return 0;
}

int MFS_Creat(int pinum, int type, char *name) {
    return 0;
}

int MFS_Unlink(int pinum, char *name) {
    return 0;
}

int MFS_Shutdown() {
    printf("MFS Shutdown\n");
    return 0;
}

