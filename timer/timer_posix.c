/*
* @Author: matthew.lan
* @Date:   2018-06-24
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-24
*/


/**********************************************************
 * Includes
**********************************************************/
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "timer.h"
#include "log/log.h"


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [timer_posix_pthread_func description]
 *
 * @param  sv  [description]
 */
static void timer_posix_pthread_func(union sigval sv)
{
    struct timer_desc *desc = (struct timer_desc *)sv.sival_ptr;
    Q_DEBUG_LOG(TAG_TIMER, "desc: %p", desc);
    if (desc && desc->func) {
        desc->func(desc->data);
    }
}

/**
 * [timer_fd_create description]
 * create timer handle.
 *
 * @param   desc  [timer description]
 *
 * @return        [success: 0; error: -1]
 */
int timer_posix_create(struct timer_desc *desc)
{
    int             ret       = -1;
    timer_t         *timer_id = NULL;
    struct sigevent sev       = {
            .sigev_notify            = SIGEV_THREAD,
            .sigev_notify_function   = timer_posix_pthread_func,
            .sigev_notify_attributes = NULL,
            .sigev_value.sival_ptr   = desc,
        };

    do {
        if (!desc) {
            break;
        }
        if (!(timer_id = (timer_t *)malloc(sizeof(timer_t)))) {
            break;
        }
        if (timer_create(CLOCK_MONOTONIC, &sev, timer_id)) {
            free(timer_id);
            break;
        }
        desc->timer_id = (int)timer_id;
        ret            = 0;
    } while (0);
    Q_DEBUG_LOG(TAG_TIMER, "ret: %d", ret);
    return ret;
}

/**
 * [timer_posix_close description]
 * close timer handle.
 *
 * @param   desc  [timer description]
 *
 * @return        [success: 0; error: -1]
 */
int timer_posix_close(struct timer_desc *desc)
{
    int     ret       = -1;
    timer_t *timer_id = NULL;

    do {
        if (!desc) {
            break;
        }
        if (!(desc->timer_id)) {
            ret = 0;
            break;
        }
        timer_id = (timer_t *)desc->timer_id;
        if (timer_delete(*timer_id)) {
            break;
        }
        ret            = 0;
        desc->timer_id = 0;
        free(timer_id);
    } while (0);
    Q_DEBUG_LOG(TAG_TIMER, "ret: %d", ret);
    return ret;
}

/**
 * [timer_posix_start description]
 * start timer.
 *
 * @param   desc       [tiemr description]
 * @param   ms         [interval time]
 * @param   reload_ms  [reload interval time]
 *
 * @return             [success: 0; error: -1]
 */
int timer_posix_start(struct timer_desc *desc, uint64_t ms, uint64_t reload_ms)
{
    int     ret       = -1;
    timer_t *timer_id = NULL;

    do {
        if (!desc \
            || (0 == desc->timer_id)) {
            break;
        }
        struct itimerspec spec = {
            .it_value = {
                .tv_sec  = ms /1000,
                .tv_nsec = ms %1000 *1000000,
            },
            .it_interval = {
                .tv_sec  = reload_ms /1000,
                .tv_nsec = reload_ms %1000 *1000000,
            },
        };
        timer_id = (timer_t *)desc->timer_id;
        if (timer_settime(*timer_id, 0, &spec, NULL)) {
            break;
        }
        ret = 0;
    } while (0);
    Q_DEBUG_LOG(TAG_TIMER, "ret: %d", ret);
    return ret;
}

/**
 * [timer_posix_stop description]
 * stop timer.
 *
 * @param   desc  [timer description]
 *
 * @return        [success: 0; error: -1]
 */
int timer_posix_stop(struct timer_desc *desc)
{
    int ret = -1;
    timer_t *timer_id = NULL;

    do {
        if (!desc \
            || (0 == desc->timer_id)) {
            break;
        }
        struct itimerspec spec;
        memset(&spec, 0, sizeof(spec));
        timer_id = (timer_t *)desc->timer_id;
        if (timer_settime(*timer_id, 0, &spec, NULL)) {
            break;
        }
        ret = 0;
    } while (0);
    Q_DEBUG_LOG(TAG_TIMER, "ret: %d", ret);
    return ret;
}

/**
 * [timer_posix_gettime description]
 * get the remaining time of timer.
 *
 * @param   desc  [timer description]
 * @param   ms    [remaining time]
 *
 * @return        [success: 0; error: -1]
 */
int timer_posix_gettime(struct timer_desc *desc, uint64_t *ms)
{
    int ret = -1;
    timer_t *timer_id = NULL;

    do {
        if (!desc \
            || (0 == desc->timer_id)) {
            break;
        }
        struct itimerspec spec;
        memset(&spec, 0, sizeof(spec));
        timer_id = (timer_t *)desc->timer_id;
        if (timer_gettime(*timer_id, &spec)) {
            break;
        }
        *ms = spec.it_value.tv_sec *1000 + spec.it_value.tv_nsec /1000000;
        ret = 0;
    } while (0);
    Q_DEBUG_LOG(TAG_TIMER, "ms: %llu, ret: %d", *ms, ret);
    return ret;
}

/**
 * [timer_posix_register description]
 * register timer include create and start.
 *
 * @param   desc       [timer description]
 * @param   ms         [interval time]
 * @param   reload_ms  [reload interval time]
 *
 * @return             [success: 0; error: -1]
 */
int timer_posix_register(struct timer_desc *desc, uint64_t ms, uint64_t reload_ms)
{
    int ret = -1;

    if (!timer_posix_create(desc) \
        && !timer_posix_start(desc, ms, reload_ms)) {
        ret = 0;
    }
    Q_DEBUG_LOG(TAG_TIMER, "ret: %d", ret);
    return ret;
}

/**
 * [timer_posix_unregister description]
 * unregister timer include stop and close.
 *
 * @param   desc  [timer description]
 *
 * @return        [success: 0; error: -1]
 */
int timer_posix_unregister(struct timer_desc *desc)
{
    int ret = -1;

    if (!timer_posix_stop(desc) \
        && !timer_posix_close(desc)) {
        ret = 0;
    }
    Q_DEBUG_LOG(TAG_TIMER, "ret: %d", ret);
    return ret;
}
