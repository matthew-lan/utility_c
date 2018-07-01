/*
* @Author: matthew.lan
* @Date:   2018-06-30
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-30
*/


#ifndef MQ_H
#define MQ_H


#if defined MQ_SYSTEMV
/**********************************************************
 * msg queue of system V.
**********************************************************/
#include "mq_sysv_usr.h"


static inline int mq_create(mq_t *mq_hdl, int indicate, long maxmsg, long msgsize)
{
    return mq_vusr_create(mq_hdl, indicate, maxmsg, msgsize, FLAG_BLOCK);
}

static inline int mq_destroy(mq_t *mq_hdl, int indicate)
{
    return mq_vusr_destroy(mq_hdl, indicate);
}

static inline int mq_sendmsg(mq_t *mq_hdl, long to, const void *data, size_t size)
{
    return mq_vusr_sendmsg(mq_hdl, to, data, size);
}

static int mq_recvmsg(int *mq_hdl, long from, void *data, size_t size)
{
    return mq_vusr_recvmsg(mq_hdl, from, data, size);
}

#elif defined MQ_POSIX


#endif


#endif // MQ_H
