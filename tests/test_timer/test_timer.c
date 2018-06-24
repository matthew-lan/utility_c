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


static void timer_cb(void *data)
{
    uint64_t remaining;
    Q_DEBUG_LOG(TAG_TEST, "data: %p", data);
    timer_fd_gettime(&desc_1, &remaining);
    timer_fd_unregister((struct timer_desc *)data);

    exit(0);
}

static void test_timer_exec_none(void)
{
    uint64_t remaining;

    desc_1.data = (void *)&desc_1;
    desc_1.func = timer_cb;
    desc_1.exec_type = EXEC_TYPE_NONE;

    // timer_fd_register(&desc_1, 5000, 5000);
    timer_fd_register(&desc_1, 3000, 0);
    timer_fd_gettime(&desc_1, &remaining);
}

static void test_timer_exec_pthread(void)
{
    desc_2.data = (void *)&desc_2;
    desc_2.func = timer_cb;
    desc_2.exec_type = EXEC_TYPE_PTHREAD;

    timer_fd_register(&desc_2, 5000, 0);
}

int main(int argc, char const *argv[])
{
    log_tags_add(TAG_TEST);
    log_tags_add(TAG_TIMER);

    // test_timer_exec_none();
    test_timer_exec_pthread();

    while (1);
    return 0;
}
