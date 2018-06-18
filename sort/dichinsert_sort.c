/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-18
*/


/*
 * 二分插入排序: (dichotomy insertion)
 *
 * 1. 插入排序的改进;
 * 2. 采用二分查找法来减少比较操作的次数;
 */

/*
 * 分类         :  内部比较排序
 * 数据结构      : 数组
 * 最差时间复杂度 : O(n^2)
 * 最优时间复杂度 : O(nlogn)
 * 平均时间复杂度 : O(n^2)
 * 所需辅助空间  : O(1)
 * 稳定性       : 稳定
 */

/**********************************************************
 * Includes
**********************************************************/
#include <string.h>
#include <stdlib.h>

#include "sort.h"


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [dichinsert_sort description]
 * 二分插入排序.
 *
 * @param  base    [starting address of array]
 * @param  nmemb   [size of array members]
 * @param  size    [size of member]
 * @param  compar  [compare function]
 */
void dichinsert_sort(void *base, size_t nmemb, size_t size,
                     int (*compar)(const void *, const void *)
                     )
{
    char *mem = (char *)malloc(size);

    do {
        if (!mem) {
            break;
        }

        for (size_t i = 1; i < nmemb; i++) {
            size_t t     = i;
            size_t left  = 0;
            size_t right = i -1;

            /* save what will be compared */
            memcpy(mem, Q_ADDR(base, t, size), size);

            while (left <= right) {
                size_t mid = (left + right) /2;
                if (compar(Q_ADDR(base, mid, size), mem)) {
                    if (!right) {
                        break;
                    }
                    right = mid -1;
                }
                else {
                    left = mid +1;
                }
            }
            for (size_t j = i; j > left; j--) {
                memcpy(Q_ADDR(base, j, size), Q_ADDR(base, j -1, size), size);
            }
            memcpy(Q_ADDR(base, left, size), mem, size);
        }
    } while (0);
    if (mem) {
        free(mem);
    }
}
