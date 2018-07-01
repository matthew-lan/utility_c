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


static inline int mq_create(mq_t *hdl, int indicate, long maxmsg, long msgsize)
{
    return mq_vusr_create(hdl, indicate, maxmsg, msgsize, FLAG_BLOCK);
}

static inline int mq_destroy(mq_t *hdl, int indicate)
{
    return mq_vusr_destroy(hdl, indicate);
}

static inline int mq_sendmsg(mq_t *hdl, long to, const void *data, size_t size)
{
    return mq_vusr_sendmsg(hdl, to, data, size);
}

static int mq_recvmsg(int *hdl, long from, void *data, size_t size)
{
    return mq_vusr_recvmsg(hdl, from, data, size);
}

#elif defined MQ_POSIX


#endif


#endif // MQ_H
