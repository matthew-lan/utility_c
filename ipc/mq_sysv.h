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
#include "mq.h"


/**********************************************************
 * Fucntions
**********************************************************/
/* create a msg queue */
extern int mq_sysv_create(int msgkey, long maxmsg, long msgsize, int flag);

/* destroy the specified queue */
extern int mq_sysv_destroy(int mq_id);

/* malloc a msg struct */
extern struct ipc_msg *mq_sysv_calloc(void);

/* free the msg struct */
extern void mq_sysv_free(struct ipc_msg *msgp);

/* send msg */
extern int mq_sysv_snd(int mq_id, struct ipc_msg *msgp);

/* receive msg */
extern int mq_sysv_rcv(int mq_id, struct ipc_msg *msgp);



/* create msg queue handle */
static inline int mq_create_v(int *mq_hdl, int msgkey, long maxmsg, long msgsize)
{
    *mq_hdl = mq_sysv_create(msgkey, maxmsg, msgsize, FLAG_BLOCK);
    return -1 == *mq_hdl ? -1 : 0;
}

/* destroy msq queue handle */
static inline int mq_destroy_v(int *mq_hdl)
{
    int ret = -1;
    if (!(ret = mq_sysv_destroy(*mq_hdl))) {
        *mq_hdl = -1;
    }
    return ret;
}

/* send data msg to [to], [to] must >0 */
extern int mq_sendmsg_v(int *mq_hdl, long to, const void *data, size_t size);

/* receive data from [from], [from] must >0 */
extern int mq_recvmsg_v(int *mq_hdl, long from, void *data, size_t size);


#endif // MQ_SYSV_H
