/*
* @Author: matthew.lan
* @Date:   2018-06-23
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-24
*/

#include <stdlib.h>
#include "log/log.h"
#include "timer/timer.h"

#define TAG_TEST    "TEST"


static struct timer_desc desc_1;
static struct timer_desc desc_2;

static struct timer_desc desc_3;
static struct timer_desc desc_4;


static void timer_cb(void *data)
{
    uint64_t          remaining;
    struct timer_desc *desc = (struct timer_desc *)data;

    Q_DEBUG_LOG(TAG_TEST, "desc: %p", desc);
    timer_fd_gettime(desc, &remaining);
    timer_fd_unregister(desc);

    // exit(0);
}

static void test_timer_exec_none(void)
{
    uint64_t remaining;

    desc_1.data      = (void *)&desc_1;
    desc_1.func      = timer_cb;
    desc_1.exec_type = EXEC_TYPE_NONE;

    Q_DEBUG_LOG(TAG_TEST, "desc_1: %p", &desc_1);
    // timer_fd_register(&desc_1, 5000, 5000);
    timer_fd_register(&desc_1, 3000, 0);
    timer_fd_gettime(&desc_1, &remaining);
}

static void test_timer_exec_pthread(void)
{
    desc_2.data      = (void *)&desc_2;
    desc_2.func      = timer_cb;
    desc_2.exec_type = EXEC_TYPE_THREAD;

    Q_DEBUG_LOG(TAG_TEST, "desc_2: %p", &desc_2);
    timer_fd_register(&desc_2, 5000, 0);
}


static void timer_posix_cb(void *data)
{
    uint64_t          remaining;
    struct timer_desc *desc = (struct timer_desc *)data;

    Q_DEBUG_LOG(TAG_TEST, "desc: %p", desc);
    timer_posix_gettime(desc, &remaining);
    timer_posix_unregister(desc);

    // exit(0);
}

static void test_timer_posix(void)
{
    uint64_t remaining;

    desc_3.data = (void *)&desc_3;
    desc_3.func = timer_posix_cb;
    Q_DEBUG_LOG(TAG_TEST, "desc_3: %p", &desc_3);
    timer_posix_register(&desc_3, 8000, 0);
    timer_posix_gettime(&desc_3, &remaining);

    desc_4.data = (void *)&desc_4;
    desc_4.func = timer_posix_cb;
    Q_DEBUG_LOG(TAG_TEST, "desc_4: %p", &desc_4);
    timer_posix_register(&desc_4, 10000, 0);
    timer_posix_gettime(&desc_4, &remaining);
}

int main(int argc, char const *argv[])
{
    log_tags_add(TAG_TEST);
    log_tags_add(TAG_TIMER);

    test_timer_exec_none();
    test_timer_exec_pthread();
    test_timer_posix();

    while (1);
    return 0;
}
