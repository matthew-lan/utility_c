/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-17
*/

#ifndef SWAP_H
#define SWAP_H

/**********************************************************
 * Includes
**********************************************************/
#include <stddef.h>


/**********************************************************
 * Fucntions
**********************************************************/
/* swap x and y of size SIZE */
#define Q_SWAP(x, y, size) \
        do { \
            size_t __size = (size); \
            char *__x = (x); \
            char *__y = (y); \
            do { \
                char t = *__x; \
                *__x++ = *__y; \
                *__y++ = t; \
            } while (0 < (--__size)); \
        } while (0)


#define Q_ADDR(base, index, size) \
        (((char *)(base)) + (index) * (size))


#endif // SWAP_H
