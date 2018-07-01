/*
* @Author: matthew.lan
* @Date:   2018-07-01
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-07-01
*/


#ifndef MQ_POSIX_USR_H
#define MQ_POSIX_USR_H

/**********************************************************
 * Includes
**********************************************************/
#include "mq_posix.h"


/**********************************************************
 * Types
**********************************************************/
typedef mqd_t   mq_t;


/**********************************************************
 * Fucntions
**********************************************************/
/* create a msg queue for user */
extern int mq_pusr_create(mq_t *mq_hdl, const char *name, long maxmsg, long msgsize, int flag);

/* destroy the specified queue for user */
extern int mq_pusr_destroy(mq_t *mq_hdl, const char *name);

/* send msg for user */
extern int mq_pusr_sendmsg(mq_t *mq_hdl, long to, const void *data, size_t size);

/* receive msg for user */
extern int mq_pusr_recvmsg(mq_t *mq_hdl, long from, void *data, size_t size);


#endif // MQ_POSIX_USR_H
