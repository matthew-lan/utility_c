/*
* @Author: matthew.lan
* @Date:   2018-06-30
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-30
*/


#ifndef IPC_TYPES_H
#define IPC_TYPES_H

/**********************************************************
 * Includes
**********************************************************/
#include <stddef.h>


/**********************************************************
 * Macros
**********************************************************/
#define TAG_MQ          "MQ"

#define FLAG_BLOCK      0
#define FLAG_NOBLOCK    1


/**********************************************************
 * Types
**********************************************************/
struct ipc_msg
{
    long msgtype;
    char data[0];
};

#define IPC_MSG_HEAD_SIZE   sizeof(struct ipc_msg)


#endif // IPC_TYPES_H
