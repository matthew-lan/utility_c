/*
* @Author: matthew.lan
* @Date:   2018-07-01
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-07-01
*/


/**********************************************************
 * Includes
**********************************************************/
#include <stdlib.h>
#include <string.h>

#include "mq_posix_usr.h"
#include "log/log.h"
#include "mq_posix.h"


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [mq_pusr_create description]
 * create a msg queue for user.
 *
 * @param   mq_hdl   [mq handle]
 * @param   name     [mq identification]
 * @param   maxmsg   [max of messages]
 * @param   msgsize  [max message size (bytes)]
 * @param   flag     [FLAG_BLOCK / FLAG_NOBLOCK]
 *
 * @return           [success: 0; error: -1]
 */
int mq_pusr_create(mq_t *mq_hdl, const char *name, long maxmsg, long msgsize, int flag)
{
    int ret = -1;
    do {
        if (!mq_hdl \
            || !name \
            || strchr(name, '/') != name) {
            break;
        }
        if (INVALID_MQDES == (*mq_hdl = mq_create_p(name, maxmsg, msgsize, flag))) {
            break;
        }
        ret = 0;
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}

/**
 * [mq_pusr_destroy description]
 * destroy the specified queue for user.
 *
 * @param   mq_hdl  [mq handle]
 * @param   name    [mq identification]
 *
 * @return          [success: 0; error: -1]
 */
int mq_pusr_destroy(mq_t *mq_hdl, const char *name)
{
    int ret = -1;
    do {
        if (INVALID_MQDES == *mq_hdl \
            || !name \
            || strchr(name, '/') != name) {
            break;
        }
        ret = mq_destroy_p(*mq_hdl, name);
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}

/**
 * [mq_pusr_sendmsg description]
 * send msg for user.
 *
 * @param   mq_hdl  [mq handle]
 * @param   to      [destination]
 * @param   data    [user send data]
 * @param   size    [size of data]
 *
 * @return          [success: 0; error: -1]
 */
int mq_pusr_sendmsg(mq_t *mq_hdl, long to, const void *data, size_t size)
{
    int ret = -1;
    do {
        if (INVALID_MQDES == *mq_hdl \
            || !data \
            || !size) {
            break;
        }
        if (-1 == mq_sendmsg_p(*mq_hdl, (const char *)data, size)) {
            break;
        }
        ret = 0;
        (void)to;
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}

/**
 * [mq_pusr_recvmsg description]
 * receive msg for user.
 *
 * @param   mq_hdl  [mq handle]
 * @param   from    [source]
 * @param   data    [user accepted data]
 * @param   size    [size of data]
 *
 * @return          [success: 0; error: -1]
 */
int mq_pusr_recvmsg(mq_t *mq_hdl, long from, void *data, size_t size)
{
    int  ret     = -1;
    char *ptr    = NULL;
    long msgsize = 0;

    do {
        if (INVALID_MQDES == *mq_hdl \
            || !data \
            || !size) {
            break;
        }
        msgsize = mq_msgsize_p(*mq_hdl);
        if (!(ptr = (char *)calloc(1, msgsize))) {
            break;
        }
        if (-1 == mq_recvmsg_p(*mq_hdl, ptr, msgsize)) {
            break;
        }
        memcpy(data, ptr, size);
        ret = 0;
        (void)from;
    } while (0);
    if (ptr) {
        free(ptr);
    }
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}
