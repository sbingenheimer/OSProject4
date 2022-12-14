#include <stdio.h>
#include "udp.h"
#include "mfs.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <string.h>
#include <time.h>


#define BUFFER_SIZE (4096)
message_t ret;
int sd;
int fc;
struct sockaddr_in addrSnd, addrRcv;
//fd_set rfds;
//struct timeval tv;


//should prob make a send function so we dont have to rewrite every method when sending to and from client and server
//
//

int sendAndRecieve (message_t message){

    int rc;
    rc = UDP_Write(sd, &addrSnd, (char*) &message, BUFFER_SIZE);
 
            if (rc == 0 && message.mtype != 8){
             rc = UDP_Read(sd, &addrRcv, (char*) &ret , BUFFER_SIZE);
             printf("client:: got reply [size: contents:()\n");
             return rc;
            }else{
                return 0;
            }

    return -1;
}


//   THINKING WE'LL DO ERROR CATCHING IN SERVER CODE   
//   ALL OF THESE SEE WHAT RETURN CODE IS WHICH DECIDES 0 OR -1. 
//   REAL ERRORS WILL BE CAUGHT IN SERVER WHICH WILL SET RET CODE

int MFS_Init(char *hostname, int port) {
    
    int MIN_PORT = 20000;
    int MAX_PORT = 40000;

    srand(time(0));
    int port_num = (rand() % (MAX_PORT - MIN_PORT) + MIN_PORT);


    // Bind random client port number
     sd = UDP_Open(port_num);
    
    //Think this is how you initialize???
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
    message.mtype = 2;
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
    message.mtype = 3;
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
    message.mtype = 4;
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
    message.mtype = 5;
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

    int count = 0;
    for (int i = 0; name[i]; i++){
        count++;
    }
    if (count > 28){
        return -1;
    }

    if (pinum < 0){
        return -1;
    }

    message_t message;
    message.mtype = 6;
    message.inum = pinum;
    message.type = type;
    message.message = name;
    //memcpy(message.message, name, sizeof(char))

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
    message.mtype = 7;
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
    message.mtype = 8;

    //send to server
    int rc = sendAndRecieve(message);


    UDP_Close(sd);

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

