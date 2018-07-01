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

#include "mq_sysv_usr.h"
#include "log/log.h"
#include "mq_sysv.h"


/**********************************************************
 * Variables
**********************************************************/
static long g_msgsize = 0;
static int  g_flag    = 0;


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [mq_vusr_create description]
 * create a msg queue for user.
 *
 * @param   mq_hdl   [mq handle]
 * @param   msgkey   [mq identification]
 * @param   maxmsg   [max of messages]
 * @param   msgsize  [max message size (bytes)]
 * @param   flag     [FLAG_BLOCK / FLAG_NOBLOCK]
 *
 * @return           [success: 0; error: -1]
 */
int mq_vusr_create(mq_t *mq_hdl, int msgkey, long maxmsg, long msgsize, int flag)
{
    int ret = -1;
    do {
        if (!mq_hdl) {
            break;
        }
        if (INVALID_MQ_ID == (*mq_hdl = mq_create_v(msgkey))) {
            break;
        }
        g_msgsize = msgsize;
        g_flag    = flag;
        ret       = 0;
        (void)maxmsg;
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}

/**
 * [mq_vusr_destroy description]
 * destroy the specified queue for user.
 *
 * @param   mq_hdl  [mq handle]
 * @param   msgkey  [mq identification]
 *
 * @return          [success: 0; error: -1]
 */
int mq_vusr_destroy(mq_t *mq_hdl, int msgkey)
{
    int ret = -1;
    do {
        if (!mq_hdl) {
            break;
        }
        if ((ret = mq_destroy_v(*mq_hdl))) {
            break;
        }
        *mq_hdl = INVALID_MQ_ID;
        (void)msgkey;
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}

/**
 * [mq_vusr_sendmsg description]
 * send msg for user.
 *
 * @param   mq_hdl  [mq handle]
 * @param   to      [destination]
 * @param   data    [user send data]
 * @param   size    [size of data]
 *
 * @return          [success: 0; error: -1]
 */
int mq_vusr_sendmsg(mq_t *mq_hdl, long to, const void *data, size_t size)
{
    int             ret   = -1;
    struct msg_sysv *msgp = NULL;
    do {
        if (INVALID_MQ_ID == *mq_hdl \
            || g_msgsize < size) {
            break;
        }
        if (!(msgp = (struct msg_sysv *)calloc(1, MSGV_HEAD_SIZE + g_msgsize))) {
            break;
        }
        msgp->msgtype = to;
        memcpy(msgp->data, data, size);
        if (-1 == mq_sendmsg_v(*mq_hdl, msgp, g_msgsize, g_flag)) {
            break;
        }
        ret = 0;
    } while (0);
    if (msgp) {
        free(msgp);
    }
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}

/**
 * [mq_vusr_recvmsg description]
 * receive msg for user.
 *
 * @param   mq_hdl  [mq handle]
 * @param   from    [source]
 * @param   data    [user accepted data]
 * @param   size    [size of data]
 *
 * @return          [success: 0; error: -1]
 */
int mq_vusr_recvmsg(mq_t *mq_hdl, long from, void *data, size_t size)
{
    int             ret   = -1;
    struct msg_sysv *msgp = NULL;
    do {
        if (INVALID_MQ_ID == *mq_hdl \
            || g_msgsize < size) {
            break;
        }
        if (!(msgp = (struct msg_sysv *)calloc(1, MSGV_HEAD_SIZE + g_msgsize))) {
            break;
        }
        msgp->msgtype = from;
        if (-1 == mq_recvmsg_v(*mq_hdl, msgp, g_msgsize, g_flag)) {
            break;
        }
        memcpy(data, msgp->data, size);
        ret = 0;
    } while (0);
    if (msgp) {
        free(msgp);
    }
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}
