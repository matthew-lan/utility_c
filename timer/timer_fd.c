/*
* @Author: matthew.lan
* @Date:   2018-06-23
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-24
*/


/**********************************************************
 * Includes
**********************************************************/
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "timer.h"
#include "log/log.h"


/**********************************************************
 * Macros
**********************************************************/
#define TIMER_EVENT_MAX     32


/**********************************************************
 * Variables
**********************************************************/
static int is_init = 0;
static int epfd    = 0;


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [timer_fd_execute description]
 * execute user's callback function.
 *
 * @param  ptr  [struct timer_desc *]
 */
static void timer_fd_execute(void *ptr)
{
    struct timer_desc *desc = (struct timer_desc *)ptr;
    uint64_t          data;
    do {
        if (!desc || !(desc->func)) {
            break;
        }
        read(desc->timer_id, &data, sizeof(data));
        Q_DEBUG_LOG(TAG_TIMER, "exec_type: %d", desc->exec_type);
        if (EXEC_TYPE_NONE == desc->exec_type) {
            desc->func(desc->data);
        }
        else if (EXEC_TYPE_PTHREAD == desc->exec_type) {
            pthread_t thread;
            if (!pthread_create(&thread, NULL, desc->func, desc->data)) {
                pthread_detach(thread);
            }
        }
    } while (0);
}

/**
 * [timer_fd_pthread_start description]
 * the pthread to listen timers.
 *
 * @param   arg  [description]
 *
 * @return       [description]
 */
static void *timer_fd_pthread_start(void *arg)
{
    struct epoll_event *events = NULL;
    int                nfds    = 0;
    int                i       = 0;

    if (!(events = (struct epoll_event *)calloc(1 \
                    , TIMER_EVENT_MAX * sizeof(struct epoll_event)))) {
        goto err_calloc;
    }
    if (-1 == (epfd = epoll_create(TIMER_EVENT_MAX)))
    {
        goto err_create;
    }
    is_init = 1;
    Q_DEBUG_LOG(TAG_TIMER, "epfd: %d", epfd);

    while (1) {
        if (0 > (nfds = epoll_wait(epfd, events, TIMER_EVENT_MAX, -1))) {
            break;
        }
        for (i = 0; i < nfds; i++) {
            if (EPOLLIN & events[i].events) {
                timer_fd_execute(events[i].data.ptr);
            }
        }
    }

    close(epfd);
err_create:
    free(events);
err_calloc:
    is_init = 0;
    exit(1);
    return NULL;
}

/**
 * [timer_fd_init description]
 * initialize timer listening pthread.
 *
 * @return    [success: 0; error: -1]
 */
static int timer_fd_init(void)
{
    int ret = 0;
    do {
        if (is_init) {
            break;
        }

        pthread_t thread;
        if (pthread_create(&thread, NULL, timer_fd_pthread_start, NULL)) {
            ret = -1;
            break;
        }
        pthread_detach(thread);
        if (!is_init) {
            usleep(1000);
        }
    } while (0);
    Q_DEBUG_LOG(TAG_TIMER, "is_init: %d, ret: %d", is_init, ret);
    return ret;
}

/**
 * [timer_fd_ctl description]
 * control timer_id.
 *
 * @param   op        [operation mode]
 * @param   timer_id  [timer handle]
 * @param   data      [private data]
 *
 * @return            [success: 0; error: -1]
 */
static int timer_fd_ctl(int op, int timer_id, void *data)
{
    int ret = -1;
    struct epoll_event event = {
        .events = EPOLLIN | EPOLLET,
        .data   = data,
    };
    ret = epoll_ctl(epfd, op, timer_id, &event);
    Q_DEBUG_LOG(TAG_TIMER, "op: %d, timer_id: %d, ret: %d", op, timer_id, ret);
    return ret;
}

#define timer_fd_ctl_add(timer_id, data) \
        timer_fd_ctl(EPOLL_CTL_ADD, timer_id, data)

#define timer_fd_ctl_del(timer_id, data) \
        timer_fd_ctl(EPOLL_CTL_DEL, timer_id, data)


/**
 * [timer_fd_create description]
 * create timer handle.
 *
 * @param   desc  [timer description]
 *
 * @return        [success: 0; error: -1]
 */
int timer_fd_create(struct timer_desc *desc)
{
    int ret = -1;
    do {
        if (timer_fd_init()) {
            break;
        }
        if (!desc) {
            break;
        }
        if (-1 == (desc->timer_id = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK))) {
            break;
        }
        ret = 0;
        Q_DEBUG_LOG(TAG_TIMER, "timer_id: %d", desc->timer_id);
    } while (0);
    Q_DEBUG_LOG(TAG_TIMER, "ret: %d", ret);
    return ret;
}

/**
 * [timer_fd_close description]
 * close timer handle.
 *
 * @param   desc  [timer description]
 *
 * @return        [success: 0; error: -1]
 */
int timer_fd_close(struct timer_desc *desc)
{
    int ret = -1;
    do {
        if (!desc \
            || (-1 == desc->timer_id)) {
            break;
        }
        ret = close(desc->timer_id);
    } while (0);
    return ret;
}

/**
 * [timer_fd_start description]
 * start timer.
 *
 * @param   desc       [tiemr description]
 * @param   ms         [interval time]
 * @param   reload_ms  [reload interval time]
 *
 * @return             [success: 0; error: -1]
 */
int timer_fd_start(struct timer_desc *desc, uint64_t ms, uint64_t reload_ms)
{
    int ret = -1;
    do {
        if (!desc \
            || (-1 == desc->timer_id)) {
            break;
        }
        struct itimerspec spec = {
            .it_value = {
                .tv_sec = ms /1000,
                .tv_nsec = ms %1000 *1000000,
            },
            .it_interval = {
                .tv_sec = reload_ms /1000,
                .tv_nsec = reload_ms %1000 *1000000,
            },
        };
        if (timer_fd_ctl_add(desc->timer_id, desc)) {
            break;
        }
        if (timerfd_settime(desc->timer_id, 0, &spec, NULL)) {
            break;
        }
        ret = 0;
    } while (0);
    Q_DEBUG_LOG(TAG_TIMER, "ms: %llu, reload_ms: %llu, ret: %d", ms, reload_ms, ret);
    return ret;
}

/**
 * [timer_fd_stop description]
 * stop timer.
 *
 * @param   desc  [timer description]
 *
 * @return        [success: 0; error: -1]
 */
int timer_fd_stop(struct timer_desc *desc)
{
    int ret = -1;
    do {
        if (!desc \
            || (-1 == desc->timer_id)) {
            break;
        }
        struct itimerspec spec;
        memset(&spec, 0, sizeof(spec));
        if (timer_fd_ctl_del(desc->timer_id, desc)) {
            break;
        }
        if (timerfd_settime(desc->timer_id, 0, &spec, NULL)) {
            break;
        }
        ret = 0;
    } while (0);
    Q_DEBUG_LOG(TAG_TIMER, "ret: %d", ret);
    return ret;
}

/**
 * [timer_fd_gettime description]
 * get the remaining time of timer.
 *
 * @param   desc  [timer description]
 * @param   ms    [remaining time]
 *
 * @return        [success: 0; error: -1]
 */
int timer_fd_gettime(struct timer_desc *desc, uint64_t *ms)
{
    int ret = -1;
    do {
        if (!desc \
            || (-1 == desc->timer_id)) {
            break;
        }
        struct itimerspec spec;
        memset(&spec, 0, sizeof(spec));
        if (timerfd_gettime(desc->timer_id, &spec)) {
            break;
        }
        *ms = spec.it_value.tv_sec *1000 + spec.it_value.tv_nsec /1000000;
        ret = 0;
    } while (0);
    Q_DEBUG_LOG(TAG_TIMER, "ms: %llu, ret: %d", *ms, ret);
    return ret;
}

/**
 * [timer_fd_register description]
 * register timer include create and start.
 *
 * @param   desc       [timer description]
 * @param   ms         [interval time]
 * @param   reload_ms  [reload interval time]
 *
 * @return             [success: 0; error: -1]
 */
int timer_fd_register(struct timer_desc *desc, uint64_t ms, uint64_t reload_ms)
{
    int ret = -1;

    if (!timer_fd_create(desc) \
        && !timer_fd_start(desc, ms, reload_ms)) {
        ret = 0;
    }
    Q_DEBUG_LOG(TAG_TIMER, "ret: %d", ret);
    return ret;
}

/**
 * [timer_fd_unregister description]
 * unregister timer include stop and close.
 *
 * @param   desc  [timer description]
 *
 * @return        [success: 0; error: -1]
 */
int timer_fd_unregister(struct timer_desc *desc)
{
    int ret = -1;

    if (!timer_fd_stop(desc) \
        && !timer_fd_close(desc)) {
        ret = 0;
    }
    Q_DEBUG_LOG(TAG_TIMER, "ret: %d", ret);
    return ret;
}
