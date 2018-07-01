/*
* @Author: matthew.lan
* @Date:   2018-07-01
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-07-01
*/


#ifndef MQ_SYSV_USR_H
#define MQ_SYSV_USR_H

/**********************************************************
 * Includes
**********************************************************/
#include "ipc_types.h"


/**********************************************************
 * Types
**********************************************************/
typedef int     mq_t;


/**********************************************************
 * Fucntions
**********************************************************/
/* create a msg queue for user */
extern int mq_vusr_create(mq_t *mq_hdl, int msgkey, long maxmsg, long msgsize, int flag);

/* destroy the specified queue for user */
extern int mq_vusr_destroy(mq_t *mq_hdl, int msgkey);

/* send msg for user */
extern int mq_vusr_sendmsg(mq_t *mq_hdl, long to, const void *data, size_t size);

/* receive msg for user */
extern int mq_vusr_recvmsg(mq_t *mq_hdl, long from, void *data, size_t size);


#endif // MQ_SYSV_USR_H
