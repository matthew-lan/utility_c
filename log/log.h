/*
* @Author: matthew.lan
* @Date:   2018-06-16
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-16
*/

#ifndef LOG_H
#define LOG_H

/**********************************************************
 * Includes
**********************************************************/


/**********************************************************
 * Macros
**********************************************************/
#define LOG_LEVEL_ERROR     3
#define LOG_LEVEL_WARN      4
#define LOG_LEVEL_INFO      6
#define LOG_LEVLE_DEBUG     7       /* default */

#define LOG_BUF_SIZE        128

#define LOG_STDOUT

#define LOG_DEBUG_ON


/**********************************************************
 * Fucntions
**********************************************************/
extern void log_level_set(int level);

extern void log_tags_set(const char *tags);

extern void log_write_stdout(int prio, const char *tag, const char *format, ...);


#ifdef  LOG_STDOUT
#define log_write       log_write_stdout

/* TODO */
#endif


#ifdef  LOG_DEBUG_ON
#define Q_ERROR_LOG(tag, format, ...) \
        log_write(LOG_LEVEL_ERROR, tag, "[%-26s] "format"\n", __func__, ##__VA_ARGS__)
#define Q_WARN_LOG(tag, format, ...) \
        log_write(LOG_LEVEL_WARN, tag, "[%-26s] "format"\n", __func__, ##__VA_ARGS__)
#define Q_INFO_LOG(tag, format, ...) \
        log_write(LOG_LEVEL_INFO, tag, "[%-26s] "format"\n", __func__, ##__VA_ARGS__)
#define Q_DEBUG_LOG(tag, format, ...) \
        log_write(LOG_LEVLE_DEBUG, tag, "[%-26s] "format"\n", __func__, ##__VA_ARGS__)

#else
#define Q_ERROR_LOG(tag, format, ...)       ((void)0)
#define Q_WARN_LOG(tag, format, ...)       ((void)0)
#define Q_INFO_LOG(tag, format, ...)       ((void)0)
#define Q_DEBUG_LOG(tag, format, ...)       ((void)0)
#endif


#endif // LOG_H
