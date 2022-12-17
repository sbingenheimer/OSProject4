#include <stdio.h>
#include "udp.h"
#include "mfs.h"
#include "message.h"

#define BUFFER_SIZE (1000)

// client code
int main(int argc, char *argv[]) {
    struct sockaddr_in addrSnd, addrRcv;

    int sd = UDP_Open(20000);
    int rc = UDP_FillSockAddr(&addrSnd, "localhost", 10000);

    message_t message;
    message.mtype = "MFS_Lookup";
    message.lookupName = "hi";

    printf("client:: send message [%s]\n", message.mtype);
    rc = UDP_Write(sd, &addrSnd, (char*)&message, BUFFER_SIZE);
    if (rc < 0) {
	printf("client:: failed to send\n");
	exit(1);
    }

    printf("client:: wait for reply...\n");
    rc = UDP_Read(sd, &addrRcv, (char*)&message, BUFFER_SIZE);
    printf("client:: got reply [size:%d contents:(%s)\n", rc, message.response);
    return 0;
}
