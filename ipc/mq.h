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
 * mq_systemv
**********************************************************/
#include "mq_sysv.h"

typedef int     mq_t;

static inline int mq_create(mq_t *mq_hdl, int msgkey, long maxmsg, long msgsize)
{
    return mq_create_v(mq_hdl, msgkey, maxmsg, msgsize);
}

static inline int mq_destroy(mq_t *mq_hdl)
{
    return mq_destroy_v(mq_hdl);
}

static inline int mq_sendmsg(mq_t *mq_hdl, long to, const void *data, size_t size)
{
    return mq_sendmsg_v(mq_hdl, to, data, size);
}

static int mq_recvmsg(int *mq_hdl, long from, void *data, size_t size)
{
    return mq_recvmsg_v(mq_hdl, from, data, size);
}

#elif defined MQ_POSIX


#endif


#endif // MQ_H
