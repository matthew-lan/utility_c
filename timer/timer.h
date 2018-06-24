/*
* @Author: matthew.lan
* @Date:   2018-06-23
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-23
*/


#ifndef TIMER_H
#define TIMER_H

/**********************************************************
 * Includes
**********************************************************/
#include <stdint.h>


/**********************************************************
 * Macros
**********************************************************/
#define TAG_TIMER       "TIMER"


/**********************************************************
 * Types
**********************************************************/
#define EXEC_TYPE_NONE      0
#define EXEC_TYPE_PTHREAD   1

/* timer descriptor */
struct timer_desc
{
    int  timer_id;          /* timer handle */
    void *data;             /* private data which be passed to func */
    void (*func)(void *);   /* callback function */
    int  exec_type;         /* how to call func:
                             *   EXEC_TYPE_NONE    : only call the function
                             *   EXEC_TYPE_PTHREAD : create a pthread to call
                             */
};


/**********************************************************
 * Fucntions
**********************************************************/
/* create timer handle */
extern int timer_fd_create(struct timer_desc *desc);

/* close timer handle */
extern int timer_fd_close(struct timer_desc *desc);

/* start timer */
extern int timer_fd_start(struct timer_desc *desc, uint64_t ms, uint64_t reload_ms);

/* stop timer */
extern int timer_fd_stop(struct timer_desc *desc);

/* get the remaining time of timer */
extern int timer_fd_gettime(struct timer_desc *desc, uint64_t *ms);

/* register timer include create and start */
extern int timer_fd_register(struct timer_desc *desc, uint64_t ms, uint64_t reload_ms);

/* unregister timer include stop and close */
extern int timer_fd_unregister(struct timer_desc *desc);


#endif // TIMER_H
