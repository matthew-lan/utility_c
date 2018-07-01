/*
* @Author: matthew.lan
* @Date:   2018-07-01
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-07-01
*/


/*
* @view
*     mq_maxmsg     <= /proc/sys/fs/mqueue/msg_max
*     mq_msgsize    <= /proc/sys/fs/mqueue/msgsize_max
*
* @modify
*     echo xxx > /proc/sys/fs/mqueue/msg_max
*/

#ifndef MQ_POSIX_H
#define MQ_POSIX_H

/**********************************************************
 * Includes
**********************************************************/
#include <mqueue.h>
#include "ipc_types.h"


/**********************************************************
 * Macros
**********************************************************/
#define INVALID_MQDES       -1


/**********************************************************
 * Fucntions
**********************************************************/
/* create a msg queue */
extern mqd_t mq_create_p(const char *name, long maxmsg, long msgsize, int flag);

/* destroy the specified queue */
extern int mq_destroy_p(mqd_t mqdes, const char *name);

/* get msgsize of the specified queue */
extern long mq_msgsize_p(mqd_t mqdes);

/* send msg */
extern ssize_t mq_sendmsg_p(mqd_t mqdes, const char *ptr, size_t len);

/* receive msg */
extern ssize_t mq_recvmsg_p(mqd_t mqdes, char *ptr, size_t len);


#endif // MQ_POSIX_H
