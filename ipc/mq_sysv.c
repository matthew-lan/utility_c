/*
* @Author: matthew.lan
* @Date:   2018-06-30
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-07-01
*/


/**********************************************************
 * Includes
**********************************************************/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
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
 * Fucntions
**********************************************************/
/**
 * [mq_create_v description]
 * create a msg queue.
 *
 * @param   msgkey  [queue identification]
 *
 * @return          [queue descriptor]
 */
int mq_create_v(int msgkey)
{
    int mq_id = msgget(msgkey, IPC_CREAT | MQ_SYSV_MODE);
    Q_DEBUG_LOG(TAG_MQ, "mq_id: %d", mq_id);
    return mq_id;
}

/**
 * [mq_destroy_v description]
 * destroy the specified queue.
 *
 * @param   mq_id  [queue descriptor]
 *
 * @return         [success: 0; error: -1]
 */
int mq_destroy_v(int mq_id)
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
 * [mq_sendmsg_v description]
 * send msg.
 *
 * @param   mq_id  [queue descriptor]
 * @param   msgp   [send data]
 * @param   size   [size of msgp->data]
 * @param   flag   [FLAG_BLOCK or not]
 *
 * @return         [success: send bytes; error: -1]
 */
ssize_t mq_sendmsg_v(int mq_id, const struct msg_sysv *msgp, size_t size, int flag)
{
    int ret = -1;
    do {
        if (-1 == mq_id \
            || !msgp \
            || 0 >= msgp->msgtype) {
            break;
        }
        ret = msgsnd(mq_id, msgp, size, FLAG_BLOCK == flag ? 0 : IPC_NOWAIT);
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return -1 == ret ? -1 : size;
}

/**
 * [mq_recvmsg_v description]
 * receive msg.
 *
 * @param   mq_id  [queue descriptor]
 * @param   msgp   [accepted data]
 * @param   size   [size of msgp->data]
 * @param   flag   [FLAG_BLOCK or not]
 *
 * @return         [success: accepted bytes; error: -1]
 */
ssize_t mq_recvmsg_v(int mq_id, struct msg_sysv *msgp, size_t size, int flag)
{
    ssize_t rdbyte = -1;
    do {
        if (-1 == mq_id \
            || !msgp \
            || 0 >= msgp->msgtype) {
            break;
        }
        rdbyte = msgrcv(mq_id, msgp, size, msgp->msgtype, FLAG_BLOCK == flag ? 0 : IPC_NOWAIT);
        if (-1 == rdbyte) {
            break;
        }
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "rdbyte: %lld", rdbyte);
    return rdbyte;
}
