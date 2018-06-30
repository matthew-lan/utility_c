/*
* @Author: matthew.lan
* @Date:   2018-06-30
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-30
*/


/**********************************************************
 * Includes
**********************************************************/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "mq_sysv.h"
#include "log/log.h"


/**********************************************************
 * Macros
**********************************************************/
/* 0666 */
#define MQ_SYSV_MODE    ( S_IRUSR | S_IWUSR \
                        | S_IRGRP | S_IWGRP \
                        | S_IROTH | S_IWOTH)


/**********************************************************
 * Variables
**********************************************************/
// static long g_maxmsg  = 0;
static long g_msgsize = 0;
static int  g_flag    = 0;


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [mq_sysv_create description]
 * create a msg queue.
 *
 * @param   msgkey   [queue identification]
 * @param   maxmsg   [max of messages on queue]
 * @param   msgsize  [max message size (bytes)]
 * @param   flag     [FLAG_BLOCK or not]
 *
 * @return           [queue descriptor]
 */
int mq_sysv_create(int msgkey, long maxmsg, long msgsize, int flag)
{
    int mq_id = -1;
    do {
        mq_id = msgget(msgkey, IPC_CREAT | MQ_SYSV_MODE);
        if (-1 == mq_id) {
            break;
        }
        // g_maxmsg  = maxmsg;
        g_msgsize = msgsize;
        g_flag    = (FLAG_NOBLOCK == flag ? IPC_NOWAIT : 0);
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "mq_id: %d", mq_id);
    return mq_id;
}

/**
 * [mq_sysv_destroy description]
 * destroy the specified queue.
 *
 * @param   mq_id  [queue descriptor]
 *
 * @return         [success: 0; error: -1]
 */
int mq_sysv_destroy(int mq_id)
{
    int ret = -1;
    do {
        if (-1 == mq_id) {
            break;
        }
        ret = msgctl(mq_id, IPC_RMID, NULL);
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}

/**
 * [mq_sysv_calloc description]
 * malloc a msg struct.
 *
 * @return    [description]
 */
struct ipc_msg *mq_sysv_calloc(void)
{
    struct ipc_msg *msgp = (struct ipc_msg *)calloc(1, g_msgsize + IPC_MSG_HEAD_SIZE);
    return msgp;
}

/**
 * [mq_sysv_free description]
 * free the msg struct.
 *
 * @param  msgp  [description]
 */
void mq_sysv_free(struct ipc_msg *msgp)
{
    if (msgp) {
        free(msgp);
    }
}



/**
 * [mq_sysv_snd description]
 * send msg.
 *
 * @param   mq_id  [queue descriptor]
 * @param   msgp   [description]
 *
 * @return         [success: 0; error: -1]
 */
int mq_sysv_snd(int mq_id, struct ipc_msg *msgp)
{
    int ret = -1;
    do {
        if (-1 == mq_id) {
            break;
        }
        ret = msgsnd(mq_id, msgp, g_msgsize, g_flag);
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}

/**
 * [mq_sysv_rcv description]
 * receive msg.
 *
 * @param   mq_id  [queue descriptor]
 * @param   msgp   [description]
 *
 * @return         [success: 0; error: -1]
 */
int mq_sysv_rcv(int mq_id, struct ipc_msg *msgp)
{
    int ret = -1;
    do {
        if (-1 == mq_id) {
            break;
        }
        ret = -1 == msgrcv(mq_id, msgp, g_msgsize, msgp->msgtype, g_flag) ? -1 : 0;
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}



/**
 * [mq_sendmsg_v description]
 * send data msg to [to].
 *
 * @param   mq_hdl  [mq handle]
 * @param   to      [destination, >0]
 * @param   data    [send data]
 * @param   size    [size of data]
 *
 * @return          [success: 0; error: -1]
 */
int mq_sendmsg_v(int *mq_hdl, long to, const void *data, size_t size)
{
    int            ret   = -1;
    struct ipc_msg *msgp = NULL;

    do {
        if (-1 == *mq_hdl) {
            break;
        }
        if (!(msgp = mq_sysv_calloc())) {
            break;
        }
        msgp->msgtype = to;
        memcpy(msgp->data, data, size);
        ret = mq_sysv_snd(*mq_hdl, msgp);
        Q_DEBUG_LOG(TAG_MQ, "mq_hdl: %d, to: %d, ret: %d", *mq_hdl, to, ret);
    } while (0);
    if (msgp) {
        mq_sysv_free(msgp);
    }
    return ret;
}

/**
 * [mq_recvmsg_v description]
 * receive data from [from].
 *
 * @param   mq_hdl  [mq handle]
 * @param   from    [source, >0]
 * @param   data    [accepted data]
 * @param   size    [size of data]
 *
 * @return          [success: 0; error: -1]
 */
int mq_recvmsg_v(int *mq_hdl, long from, void *data, size_t size)
{
    int            ret   = -1;
    struct ipc_msg *msgp = NULL;

    do {
        if (-1 == *mq_hdl) {
            break;
        }
        if (!(msgp = mq_sysv_calloc())) {
            break;
        }
        msgp->msgtype = from;
        if (!(ret = mq_sysv_rcv(*mq_hdl, msgp))) {
            memcpy(data, msgp->data, size);
        }
        Q_DEBUG_LOG(TAG_MQ, "mq_hdl: %d, from: %d, ret: %d", *mq_hdl, from, ret);
    } while (0);
    if (msgp) {
        mq_sysv_free(msgp);
    }
    return ret;
}
