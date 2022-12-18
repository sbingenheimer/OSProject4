#ifndef __message_h__
#define __message_h__

#include "mfs.h"

#define MFS_INIT (1)
#define MFS_LOOKUP (2)
#define MFS_STAT (3)
#define MFS_WRITE (4)
#define MFS_READ (5)
#define MFS_CRET (6)
#define MFS_UNLINK (7)
#define MFS_SHUTDOWN (8)


typedef struct {
    int mtype; // message type from above
    int rc;    // return code
    char *response; //buffer for any response
    // put more here ...
    int inum;
    char * lookupName;
    MFS_Stat_t *statStruct;

    int offset;
    int nbytes;
    int type;
    char *message;
    //ADDED THIS
    MFS_DirEnt_t *dirStruct;
} message_t;

#endif // __message_h__

