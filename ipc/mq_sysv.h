/*
* @Author: matthew.lan
* @Date:   2018-06-30
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-30
*/


#ifndef MQ_SYSV_H
#define MQ_SYSV_H

/**********************************************************
 * Includes
**********************************************************/
#include "ipc_types.h"


/**********************************************************
 * Types
**********************************************************/
struct msg_sysv
{
    long msgtype;
    char data[0];
};


/**********************************************************
 * Macros
**********************************************************/
#define MSGV_HEAD_SIZE      sizeof(struct msg_sysv)

#define INVALID_MQ_ID       -1


/**********************************************************
 * Fucntions
**********************************************************/
/* create a msg queue */
extern int mq_create_v(int msgkey);

/* destroy the specified queue */
extern int mq_destroy_v(int mq_id);

/* send msg */
extern ssize_t mq_sendmsg_v(int mq_id, const struct msg_sysv *msgp, size_t size, int flag);

/* receive msg */
extern ssize_t mq_recvmsg_v(int mq_id, struct msg_sysv *msgp, size_t size, int flag);


#endif // MQ_SYSV_H
