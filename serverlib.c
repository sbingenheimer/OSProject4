#include <stdio.h>
#include "udp.h"

#define BUFFER_SIZE (4096)
int sd;

// GET CALLS FROM MFS AND DO OPERATIONS THEN RETURN


// UDP read and write
// get the command from the incoming message calls cooresponding method
// in methods, we can update the input message to create a new outgoing message to return
// gets return value from the method (-1, 0) and returns that value to client
void getMSFCommand(){
    sd = UDP_Open(10000); //not sure what to do here exactly. not sure what the port should be 
    assert(sd > -1);
    while (1) {
	struct sockaddr_in addr;
	char message[BUFFER_SIZE];
	printf("server:: waiting...\n");
	int rc = UDP_Read(sd, &addr, message, BUFFER_SIZE);
	printf("server:: read message [size:%d contents:(%s)]\n", rc, message);
    
    //assuming we do something with the message in here
    //call which function we want or whatever 


	if (rc > 0) {
            char reply[BUFFER_SIZE];
            sprintf(reply, "goodbye world");
            rc = UDP_Write(sd, &addr, reply, BUFFER_SIZE);
	    printf("server:: reply\n");
	} 
    }
    return 0; 

}





int lookup (message_t message) {
    return 0;
}

int stat (message_t message) {
    return 0;
}

int write (message_t message) {
    return 0;
}

int read (message_t message) {
    return 0;
}

int unlink (message_t message) {
    return 0;
}

int shutdown (message_t message) {
    return 0;
}

