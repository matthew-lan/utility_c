/*
* @Author: matthew.lan
* @Date:   2018-07-01
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-07-01
*/


/**********************************************************
 * Includes
**********************************************************/
#include <fcntl.h>
#include <sys/stat.h>

#include "mq_posix.h"
#include "log/log.h"


/**********************************************************
 * Macros
**********************************************************/
/* 0666 */
#define MQ_POSIX_MODE   ( S_IRUSR | S_IWUSR \
                        | S_IRGRP | S_IWGRP \
                        | S_IROTH | S_IWOTH)


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [mq_create_p description]
 * create a msg queue.
 *
 * @param   name     [queue identification]
 * @param   maxmsg   [max of messages]
 * @param   msgsize  [max message size (bytes)]
 * @param   flag     [FLAG_BLOCK / FLAG_NOBLOCK]
 *
 * @return           [queue descriptor]
 */
mqd_t mq_create_p(const char *name, long maxmsg, long msgsize, int flag)
{
    mqd_t mqdes = -1;
    do {
        if (!name) {
            break;
        }
        struct mq_attr attr = {
            .mq_maxmsg  = maxmsg,
            .mq_msgsize = msgsize,
        };
        if (-1 == (mqdes = mq_open(name, O_RDWR | O_CREAT, MQ_POSIX_MODE, &attr))) {
            break;
        }
        attr.mq_flags = FLAG_BLOCK == flag ? 0 : O_NONBLOCK;
        mq_setattr(mqdes, &attr, NULL);
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "mqdes: %d", mqdes);
    return mqdes;
}

/**
 * [mq_destroy_p description]
 * destroy the specified queue.
 *
 * @param   mqdes  [queue descriptor]
 * @param   name   [description]
 *
 * @return         [success: 0; error: -1]
 */
int mq_destroy_p(mqd_t mqdes, const char *name)
{
    int ret = -1;
    do {
        if (-1 == mqdes || !name) {
            break;
        }
        if (mq_close(mqdes) \
            || mq_unlink(name)) {
            break;
        }
        ret = 0;
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return ret;
}

/**
 * [mq_msgsize_p description]
 * get msgsize of the specified queue.
 *
 * @param   mqdes  [queue descriptor]
 *
 * @return         [msgsize]
 */
long mq_msgsize_p(mqd_t mqdes)
{
    long           msgsize = 8192;
    struct mq_attr attr;
    do {
        if (-1 == mqdes \
            || mq_getattr(mqdes, &attr)) {
            break;
        }
        msgsize = attr.mq_msgsize;
    } while (0);
    return msgsize;
}

/**
 * [mq_sendmsg_p description]
 * send msg.
 *
 * @param   mqdes  [queue descriptor]
 * @param   ptr    [send data]
 * @param   len    [size of ptr]
 *
 * @return         [success: send bytes; error: -1]
 */
ssize_t mq_sendmsg_p(mqd_t mqdes, const char *ptr, size_t len)
{
    int ret = -1;
    do {
        if (-1 == mqdes \
            || !ptr \
            || !len) {
            break;
        }
        if (mq_send(mqdes, ptr, len, 0)) {
            break;
        }
        ret = 0;
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "ret: %d", ret);
    return -1 == ret ? -1 : len;
}

/**
 * [mq_recvmsg_p description]
 * receive msg.
 *
 * @param   mqdes  [queue descriptor]
 * @param   ptr    [accepted data]
 * @param   len    [size of ptr, >= mq_msgsize]
 *
 * @return         [success: accepted bytes; error: -1]
 */
ssize_t mq_recvmsg_p(mqd_t mqdes, char *ptr, size_t len)
{
    ssize_t rdbyte = -1;
    do {
        if (-1 == mqdes \
            || !ptr \
            || !len) {
            break;
        }
        rdbyte = mq_receive(mqdes, ptr, len, NULL);
    } while (0);
    Q_DEBUG_LOG(TAG_MQ, "rdbyte: %ld", rdbyte);
    return rdbyte;
}
