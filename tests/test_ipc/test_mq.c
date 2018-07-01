/*
* @Author: matthew.lan
* @Date:   2018-06-30
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-07-01
*/


#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "log/log.h"

// #define MQ_SYSTEMV
#define MQ_POSIX
#include "ipc/mq.h"

#define TAG_TEST    "TEST"


#define TEST_MAXMSG     10
#define TEST_MSGSIZE    128

#ifdef  MQ_SYSTEMV
#define TEST_MSGKEY_1   1234
#define TEST_MSGKEY_2   1235

static mq_t mq_hdl_1 = INVALID_MQ_ID;
static mq_t mq_hdl_2 = INVALID_MQ_ID;


#elif   defined MQ_POSIX
#define TEST_MSGKEY_1   "/thread_1"
#define TEST_MSGKEY_2   "/thread_2"

static mq_t mq_hdl_1 = INVALID_MQDES;
static mq_t mq_hdl_2 = INVALID_MQDES;

#endif

enum TASK_ID
{
      TASK_THREAD_1 = 1
    , TASK_THREAD_2 = 2
};


struct test_msg
{
    int  msg_id;
    char buf[120];
};

static void *test_thread_1(void *arg)
{
    const char      buf_1[] = "this is a test from thread-1.";
    struct test_msg msg_1;

    while (1) {
        memset(&msg_1, 0, sizeof(msg_1));
        mq_recvmsg(&mq_hdl_1, TASK_THREAD_1, &msg_1, sizeof(msg_1));
        Q_DEBUG_LOG(TAG_TEST, "thread_1: %d - %s\n", msg_1.msg_id, msg_1.buf);

        sleep(3);
        memset(&msg_1, 0, sizeof(msg_1));
        msg_1.msg_id = 222;
        memcpy(msg_1.buf, buf_1, strlen(buf_1));
        mq_sendmsg(&mq_hdl_2, TASK_THREAD_2, &msg_1, sizeof(msg_1));
    }
    return NULL;
}

static void *test_thread_2(void *arg)
{
    const char      buf_2[] = "this is a test from thread-2.";
    struct test_msg msg_2;

    while (1) {
        sleep(3);

        memset(&msg_2, 0, sizeof(msg_2));
        msg_2.msg_id = 111;
        memcpy(msg_2.buf, buf_2, strlen(buf_2));
        mq_sendmsg(&mq_hdl_1, TASK_THREAD_1, &msg_2, sizeof(msg_2));

        memset(&msg_2, 0, sizeof(msg_2));
        mq_recvmsg(&mq_hdl_2, TASK_THREAD_2, &msg_2, sizeof(msg_2));
        Q_DEBUG_LOG(TAG_TEST, "thread_2: %d - %s\n", msg_2.msg_id, msg_2.buf);
    }
    return NULL;
}


int main(int argc, char const *argv[])
{
    pthread_t thread;

    log_tags_add(TAG_TEST);
    log_tags_add(TAG_MQ);

    if (mq_create(&mq_hdl_1, TEST_MSGKEY_1, TEST_MAXMSG, TEST_MSGSIZE)) {
        goto err_mq_1;
    }
    if (mq_create(&mq_hdl_2, TEST_MSGKEY_2, TEST_MAXMSG, TEST_MSGSIZE)) {
        goto err_mq_2;
    }

    if (pthread_create(&thread, NULL, test_thread_1, NULL)) {
        goto err_thread;
    }
    pthread_detach(thread);

    test_thread_2(NULL);

err_thread:
    mq_destroy(&mq_hdl_2, TEST_MSGKEY_2);
err_mq_2:
    mq_destroy(&mq_hdl_1, TEST_MSGKEY_1);
err_mq_1:
    return 0;
}
