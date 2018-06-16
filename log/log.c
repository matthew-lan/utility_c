/*
* @Author: matthew.lan
* @Date:   2018-06-16
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-16
*/


/**********************************************************
 * Includes
**********************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"


/**********************************************************
 * Variables
**********************************************************/
static int  default_level = LOG_LEVLE_DEBUG;
static char *log_tags     = NULL;


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [log_level_set description]
 *
 * @param  level  [description]
 */
void log_level_set(int level)
{
    default_level = level;
}

/**
 * [log_tags_set description]
 *
 * @param  tags  [description]
 */
void log_tags_set(const char *tags)
{
    if (tags) {
        if (log_tags) {
            free(log_tags);
            log_tags = NULL;
        }
        log_tags = strdup(tags);
    }
}

/**
 * [log_write_stdout description]
 *
 * @param   prio    [description]
 * @param   tag     [description]
 * @param   format  [description]
 */
void log_write_stdout(int prio, const char *tag, const char *format, ...)
{
    char    buf[LOG_BUF_SIZE] = {0};
    size_t  len               = 0;
    va_list ap;

    do {
        if (prio > default_level) {
            break;
        }
        if (!log_tags \
            || !strstr(log_tags, tag)) {
            break;
        }

        switch (prio) {
        case LOG_LEVEL_ERROR:
            sprintf(buf, "E\\%-6s ", tag);
            break;
        case LOG_LEVEL_WARN:
            sprintf(buf, "W\\%-6s ", tag);
            break;
        case LOG_LEVEL_INFO:
            sprintf(buf, "I\\%-6s ", tag);
            break;
        default:
            sprintf(buf, "D\\%-6s ", tag);
            break;
        }
        len = strlen(buf);
        va_start(ap, format);
        vsnprintf(buf + len, LOG_BUF_SIZE - len, format, ap);
        va_end(ap);
        printf("%s", buf);
    } while (0);
}
