#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>


#include "udp.h"
#include "ufs.h"
#include "message.h"

#define BUFFER_SIZE (4096)
int sd;
int fd;
message_t touse;
struct stat buff;
char * port;
char * imageIn;
/* NOTE TO SELF
Logic revolving around the message.inum*(sizeof(inode_t)) may be subject to change.
Instead of finding the currinode directly at that inum, we might have to find the block that includes that inum by doing the some sort of the following
message.inum*(sizeof(inode_t)) - message.inum*(sizeof(inode_t)) % something
Right now my belief is that current logic is right
*/

//  *********I THINK WE NEED THE PINUM INCLUDED IN MESSAGE, just used message.inum right now when pinum was needed************

//  *********I ALSO THINK THAT IN OUR MFS WE SHOULD NOT JUST BE CHECKING THE RC, BUT UPDATING THE MESSAGE INCLUDING MFS_Stat_t and MFS_DirEnt_t************

//  *********ADDED MFS_DirEnt_t to message.h ************
int serverLookup (message_t message, void* image) {
    
   super_t *organizedImage = (super_t *) image;
    inode_t *inode_table = image + (organizedImage->inode_region_addr * UFS_BLOCK_SIZE);
    inode_t *data_table = image + (organizedImage->data_region_addr * UFS_BLOCK_SIZE);
    inode_t *root_inode = inode_table;
    //inode_t *root_data = data_table;
    inode_t *currinode;
    dir_ent_t *curr_dir;
    int flagHit = 0;
    int inumVal = -1;
    //lseek(fd, root_inode + message.inum*sizeof(inode_t), SEEK_SET);
    //read(fd, &currinode, sizeof(inode_t));

    if (message.inum < 0 || message.inum >= organizedImage->num_inodes){
        return -1;
    }
    //int parent = root_inode->direct[message.inum];


    currinode = root_inode + sizeof(inode_t)*message.inum;  

    printf("%d", currinode->type);
    
    if(root_inode->type == 1){
        return -1;
    }
    //Loops through the curr inode dirs
        for(int j = 0; j<DIRECT_PTRS; j++){    
            curr_dir = image + (currinode->direct[j] * UFS_BLOCK_SIZE);
            //logic could be wrong here****************Deleted this logic in later sections but kept it for reference in case need to add in all other functions
            int currdirectsize = (int)(sizeof(curr_dir));
            for(int k = 0; k<currdirectsize; k++){
                //if the name from our message and the dir we are looking at, return the inum
                if(strcmp(curr_dir[k].name, message.lookupName) == 0 && curr_dir[k].inum != -1){
                    inumVal = curr_dir[k].inum;
                    flagHit = 1;
                    break;
                }     
            }

            if(flagHit){
                break;
            }
        }   
    return inumVal;
    
}

message_t serverStat (message_t message, void* image) {
   message_t messageReturn = message;
    messageReturn.rc = -1;

    super_t *organizedImage = (super_t *) image;
    inode_t *inode_table = image + (organizedImage->inode_region_addr * UFS_BLOCK_SIZE);
    //inode_t *data_table = image + (organizedImage->data_region_addr * UFS_BLOCK_SIZE);
    inode_t *root_inode = inode_table;
    //inode_t *root_data = data_table;

    inode_t *currinode = root_inode + sizeof(inode_t)*message.inum;
    
    if(currinode->type != 0 && currinode->type != 1){
        return messageReturn;
    }
    messageReturn.statStruct->type = currinode->type;
    messageReturn.statStruct->size = currinode->size; 
    messageReturn.rc = 0;
    
    return messageReturn;

}

int serverWrite (message_t message, void* image) {
    /*
    super_t *organizedImage = (super_t *) image;
    
    inode_t *inode_table = image + (organizedImage->inode_region_addr * UFS_BLOCK_SIZE);
    //inode_t *data_table = image + (organizedImage->data_region_addr * UFS_BLOCK_SIZE);
    inode_t *root_inode = inode_table;
    //inode_t *root_data = data_table;
    dir_ent_t *offset_dir;
    //inode_t *offset_table;
    //int flagHit = 0;
    int success = -1;

    // **********************IS THIS LOGIC RIGHT?
    inode_t *currinode = root_inode + sizeof(inode_t)*message.inum;
    inode_t *startInode = root_inode + message.offset;
    int startingByteLoc = message.nbytes;

    if(message.nbytes <= 4096 && message.offset%(sizeof(inode_t)) == 0){
        for (int i = 0; i< organizedImage->inode_region_len; i++){
        currinode = startInode + sizeof(inode_t)*i;
            //Relate to other functions *****

            int sizeDirect = (int)(sizeof(currinode->direct)/sizeof(currinode->direct[0]));
            for(int i = 0; i<sizeDirect; i++){
                offset_dir = image + (currinode->direct[i] * UFS_BLOCK_SIZE);

                if(currinode->type != 0 && success !=0){

                    offset_dir->name = message.message;
                    currinode->size = message.nbytes;
                    success = 0;
                }

                else if(success == 0){

                    offset_dir->name = message.message;
                    offset_dir->inum = -1;
                }
                else{
                    startingByteLoc --;
                }
            }
            if(success != 0){
                break;
            }
            else if((startingByteLoc - sizeof(inode_t) <= sizeof(inode_t)*(i))){
                int remainingAllocs = (sizeof(inode_t)*i) - (startingByteLoc - sizeof(inode_t));
                for(int i = 0; i<remainingAllocs; i++){
                    offset_dir = image + (currinode->direct[i] * UFS_BLOCK_SIZE);
                    offset_dir->name = message.message;
                    offset_dir->inum = -1;
                }
            }
        }
    }

    return success;
    */
    return 0;
}

message_t serverRead (message_t message, void* image) {
    message_t messageReturn = message;
    /*
    messageReturn.rc = -1;

    super_t *organizedImage = (super_t *) image;
    inode_t *inode_table = image + (organizedImage->inode_region_addr * UFS_BLOCK_SIZE);
    //inode_t *data_table = image + (organizedImage->data_region_addr * UFS_BLOCK_SIZE);
    //inode_t *root_data = data_table;
    inode_t *root_inode = inode_table;
    dir_ent_t *offset_dir;
    //inode_t *offset_table;
    //int flagHit = 0;

    // **********************IS THIS LOGIC RIGHT?
    inode_t *currinode = root_inode + message.offset;
    if(message.nbytes <= 4096 && message.offset%(sizeof(inode_t)) == 0){

            //Relate to other functions *****
            int sizeDirect = (int)(sizeof(currinode->direct)/sizeof(currinode->direct[0]));
            for(int i = 0; i<sizeDirect; i++){
                offset_dir = image + (currinode->direct[i] * UFS_BLOCK_SIZE);
                if(offset_dir->inum !=-1){
                    messageReturn.dirStruct->name = offset_dir->name;
                    messageReturn.dirStruct->inum = offset_dir->inum;
                    messageReturn.rc = 0;
                    break;
                }
            }
    }

*/
    return messageReturn;
}

int serverCreat(message_t message, void* image){
    super_t *organizedImage = (super_t *) image;
    inode_t *inode_table = image + (organizedImage->inode_region_addr * UFS_BLOCK_SIZE);
    inode_t *data_table = image + (organizedImage->data_region_addr * UFS_BLOCK_SIZE);
    inode_t *root_inode = inode_table;
    inode_t *currinode;
    dir_ent_t *curr_dir;
    int flagHit = 0;
    int namematch = 0;
    //int inumVal = -1;
    dir_ent_t opendir;
    dir_ent_t addfile;
    dir_ent_t adddir;



    if (message.inum < 0 || message.inum > organizedImage->num_inodes){
        return -1;
    }
    

    currinode = root_inode + sizeof(inode_t)*message.inum;

    if(currinode->type == 1 || currinode->type == 0){
        return -1;
    }
    
            //Loops through the curr inode dirs
        size_t sizeDirect = (int)(sizeof(currinode->direct)/sizeof(currinode->direct[0]));
        for(int j = 0; j<sizeDirect; j++){    
            curr_dir = image + (currinode->direct[j] * UFS_BLOCK_SIZE);
            //logic could be wrong here****************Deleted this logic in later sections but kept it for reference in case need to add in all other functions
            size_t currdirectsize = (int)(sizeof(curr_dir));
            for(int k = 0; k<currdirectsize; k++){
                
                //if the name from our message and the dir we are looking at, return the inum
                if(curr_dir[k].inum == -1){
                    opendir = curr_dir[k];
                    flagHit = 1;
                    break;
                }else if(strcmp(curr_dir[k].name, message.message)){
                    namematch = 1;
                }
                    
            }
        }   

        if (namematch == 1) {
            return 0;
        }
        /*
        else if (flagHit == 1){
            if (message.type == 0){
                inode_t *newInode;
                newInode->type = 0;
                newInode->direct[0] = adddir;
                int sd = pwrite()
            }
            else if (message.type == 1){
                inode_t *newInode;
                addfile.name = message.message;
                newInode->type = 1;
                newInode->direct[0] = addfile;

            }
            */
        

    return -1;
}

int serverUnlink (message_t message, void* image) {
    /*

    super_t *organizedImage = (super_t *) image;
    inode_t *inode_table = image + (organizedImage->inode_region_addr * UFS_BLOCK_SIZE);
    //inode_t *data_table = image + (organizedImage->data_region_addr * UFS_BLOCK_SIZE);
    //inode_t *root_data = data_table;
    inode_t *root_inode = inode_table;
    dir_ent_t *offset_dir;
    //inode_t *offset_table;
    //int flagHit = 0;
    int success = -1;
    int count = 0;
    int nonexistantCount = 0;
    int totalcount = 0;
    // **********************IS THIS LOGIC RIGHT?
    inode_t *currinode = root_inode + message.inum*(sizeof(inode_t));
    
    if(currinode->type == NULL){
        return -1;
    }
    

            //Relate to other functions *****
            int sizeDirect = (int)(sizeof(currinode->direct)/sizeof(currinode->direct[0]));
            for(int i = 0; i<sizeDirect; i++){
                offset_dir = image + (currinode->direct[i] * UFS_BLOCK_SIZE);
                if(currinode->type = 0){
                    //logic for deleting dir
                    if(offset_dir->inum !=-1){
                        offset_dir->name = NULL;
                        count ++;
                    }
                    //success logic for deleting something that doesn't exist
                    else{
                        nonexistantCount++;
                    }
                }
                else if(currinode->type = 1){
                    //logic for deleting file
                    if(offset_dir->inum ==-1){
                        offset_dir->name = NULL;
                        count ++;
                    }
                }
                totalcount++;
            }

    if((count != totalcount && currinode->type == 1) || (nonexistantCount != 0 && nonexistantCount != totalcount)){
        success = -1;
    }
    else{
        success = 0;
    }
    return success;
    */
    return 0;
}

int serverShutdown (message_t message, void* image) {
    exit(0);
    return -1;
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


    int fd = open(imageIn, O_RDWR, 0644);
    if (fd < 0) {
        exit(0);
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
    assert(sd > -1);
    while (1) {
	struct sockaddr_in addr;
	printf("server:: waiting...\n");
	int rf = UDP_Read(sd, &addr, (char*)&touse, BUFFER_SIZE);
	printf("server:: read message [size: contents]\n");


   
    int resp;

    if (touse.mtype == 2) {
        resp = serverLookup(touse, image);
        touse.rc = resp;
    }
    else if (touse.mtype == 3) {
        message_t messageReturn = serverStat(touse, image);
        touse = messageReturn;
    }
    else if (touse.mtype == 4) {
        resp = serverWrite(touse, image);
        touse.rc = resp;
    }
    else if (touse.mtype == 5) {
        message_t messageReturn = serverRead(touse, image);
        touse = messageReturn;
    }
    else if (touse.mtype == 6) {
        resp = serverCreat(touse, image);
        touse.rc = resp;
    }
    else if (touse.mtype == 7) {
        resp = serverUnlink(touse, image);
        touse.rc = resp;
    }
    else if (touse.mtype == 8) {
        UDP_Close(sd);
        fsync(fd);
        resp = serverShutdown(touse, image);
        touse.rc = resp;
    }
    else {
        touse.rc = -1;
    }
    if(rf > 0){ 
	
    rf = UDP_Write(sd, &addr, (char *) &touse, BUFFER_SIZE);
	
    }

    }

    close(fd);
    return 0; 
    
}
