#include <stdio.h>
#include "udp.h"
#include "mfs.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <string.h>

#define BUFFER_SIZE (4096)
message_t ret;
int sd;
int fc;
struct sockaddr_in addrSnd, addrRcv;
fd_set rfds;
struct timeval tv;


//should prob make a send function so we dont have to rewrite every method when sending to and from client and server
//
//

int sendAndRecieve (message_t message){

    int rc;
    int selret = 0;

    while (selret == 0){

    FD_ZERO(&rfds);
    FD_SET(sd, &rfds);

    tv.tv_sec = 30;
    tv.tv_usec = 0;
    
    rc = UDP_Write(sd, &addrSnd, (char*) &message, BUFFER_SIZE);
    if (rc != -1) {
        selret = select(sd+1, &rfds, NULL, NULL, &tv);
        if (selret == -1){
            perror("select()");
        }
        else if(selret != 0){
             rc = UDP_Read(sd, &addrRcv, (char*) &ret , BUFFER_SIZE);
             printf("client:: got reply [size:%d contents:(%s)\n", rc, message.response);
             return rc;
        }
        else {
            printf("no response in 30 seconds");
        }

    }


    }

    return -1;
}


//   THINKING WE'LL DO ERROR CATCHING IN SERVER CODE   
//   ALL OF THESE SEE WHAT RETURN CODE IS WHICH DECIDES 0 OR -1. 
//   REAL ERRORS WILL BE CAUGHT IN SERVER WHICH WILL SET RET CODE

int MFS_Init(char *hostname, int port) {
    
    //Think this is how you initialize???
    sd = UDP_Open(0);
    if (sd < 0) {
        return -1;
    }
    fc = UDP_FillSockAddr(&addrSnd, hostname, port);
    if (fc < 0){
        return -1;
    }else {
       return 0;
    }
}

int MFS_Lookup(int pinum, char *name) {
    // network communication to do the lookup to server

    message_t message;
    message.mtype = "MFS_Lookup";
    message.lookupName = name;

    //send to server
    int rc = sendAndRecieve(message);

    //if successful then get the return code from server
    if (rc == 0){
        message_t resp = ret;

        //if -1 lookup failed otherwise return inum 
        if (resp.rc != -1){
            return resp.rc;
        }
        else {
            return -1;
        }
    }

    return -1;
}

int MFS_Stat(int inum, MFS_Stat_t *m) {

    message_t message;
    message.mtype = "MFS_Stat";
    message.inum = inum;

    //send to server
    int rc = sendAndRecieve(message);

    //if successful then get the return code from server
    if (rc == 0){
        message_t resp = ret;

        //if -1 lookup failed otherwise return inum 
        if (resp.rc != -1){
            memcpy(m, resp.response, sizeof(MFS_Stat_t));
            return 0;
        }
        else {
            return -1;
        }
    }

    return -1;
}

int MFS_Write(int inum, char *buffer, int offset, int nbytes) {
   
    if (inum < 0 || offset < 0 || nbytes < 0){
        return -1;
    }

    message_t message;
    message.mtype = "MFS_Write";
    message.inum = inum;
    message.offset = offset;
    message.nbytes = nbytes;
    message.message = buffer;
    //memcpy(message.message, buffer, sizeof(char));

    //send to server
    int rc = sendAndRecieve(message);

    //if successful then get the return code from server
    if (rc == 0){
        message_t resp = ret;

        //if -1 lookup failed otherwise return 0 
        if (resp.rc != -1){
            return 0;
        }
        else {
            return -1;
        }
    }

    return -1;
}

int MFS_Read(int inum, char *buffer, int offset, int nbytes) {

    if (inum < 0 || offset < 0 || nbytes < 0){
        return -1;
    }
    
    message_t message;
    message.mtype = "MFS_Read";
    message.inum = inum;
    message.offset = offset;
    message.nbytes = nbytes;

    //send to server
    int rc = sendAndRecieve(message);

    //if successful then get the return code from server
    if (rc == 0){
        message_t resp = ret;

        //if -1 lookup failed otherwise return 0 
        if (resp.rc != -1){
            memcpy(resp.response, buffer, sizeof(char));
            return 0;
        }
        else {
            return -1;
        }
    }

    return -1;
}

int MFS_Creat(int pinum, int type, char *name) {

    message_t message;
    message.mtype = "MFS_Read";
    message.inum = pinum;
    message.type = type;
    message.message = name;
    //memcpy(message.message, name, sizeof(char));

    //send to server
    int rc = sendAndRecieve(message);

    //if successful then get the return code from server
    if (rc == 0){
        message_t resp = ret;

        //if -1 lookup failed otherwise return 0 
        if (resp.rc != -1){
            return 0;
        }
        else {
            return -1;
        }
    }

    return -1;
}

int MFS_Unlink(int pinum, char *name) {

    message_t message;
    message.mtype = "MFS_Unlink";
    message.inum = pinum;
    message.message = name;
    //memcpy(message.message, name, sizeof(char));

    //send to server
    int rc = sendAndRecieve(message);

    //if successful then get the return code from server
    if (rc == 0){
        message_t resp = ret;

        //if -1 lookup failed otherwise return 0 
        if (resp.rc != -1){
            return 0;
        }
        else {
            return -1;
        }
    }

    return -1;

}

int MFS_Shutdown() {

    message_t message;
    message.mtype = "MFS_Shutdown";

    //send to server
    int rc = sendAndRecieve(message);

    //if successful then get the return code from server
    if (rc == 0){
        message_t resp = ret;

        //if -1 lookup failed otherwise return 0 
        if (resp.rc != -1){
            return 0;
        }
        else {
            return -1;
        }
    }

    return -1;
}

