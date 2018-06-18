/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-18
*/


/*
 * 希尔排序: (dichotomy insertion)
 *
 * 1. 也叫递减增量排序, 插入排序的更高效改进;
 *
 * 2. 希尔排序是基于插入排序的以下两点性质而提出改进方法的:
 *  2.1 插入排序在对几乎已经排好序的数据操作时，效率高，即可以达到线性排序的效率;
 *  2.2 但插入排序一般来说是低效的，因为插入排序每次只能将数据移动一位;
 */

/*
 * 希尔排序通过将比较的全部元素分为几个区域来提升插入排序的性能;
 * 这样可以让一个元素可以一次性地朝最终位置前进一大步;
 * 然后算法再取越来越小的步长进行排序;
 * 算法的最后一步就是普通的插入排序，但是到了这步，需排序的数据几乎是已排好的了（此时插入排序较快）;
 */

/*
 * 分类         :  内部比较排序
 * 数据结构      : 数组
 * 最差时间复杂度 : 根据步长序列的不同而不同。已知最好的为O(n(logn)^2)
 * 最优时间复杂度 : O(n)
 * 平均时间复杂度 : O根据步长序列的不同而不同
 * 所需辅助空间  : O(1)
 * 稳定性       : 不稳定
 */

/**********************************************************
 * Includes
**********************************************************/
#include <string.h>
#include <stdlib.h>

#include "sort.h"


/**********************************************************
 * Macros
**********************************************************/
#define step    3


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [shell_sort description]
 * 希尔排序.
 *
 * @param  base    [starting address of array]
 * @param  nmemb   [size of array members]
 * @param  size    [size of member]
 * @param  compar  [compare function]
 */
void shell_sort(void *base, size_t nmemb, size_t size,
                int (*compar)(const void *, const void *)
                )
{
    char   *mem = (char *)malloc(size);
    size_t h    = 0;

    do {
        if (!mem) {
            break;
        }
        while (h <= nmemb) {
            h = step * h +1;
        }

        while (1 <= h) {
            for (size_t i = h; i < nmemb; i++) {
                size_t t = i;
                size_t j = i - h;

                /* save what will be compared */
                memcpy(mem, Q_ADDR(base, t, size), size);
                do {
                    /* right shift all array left members */
                    if (compar(Q_ADDR(base, j, size), mem)) {
                        memcpy(Q_ADDR(base, j + h, size), Q_ADDR(base, j, size), size);
                    }
                    /* fix the position value and exit */
                    else {
                        j += h;
                        break;
                    }
                    /* exit */
                    if (j < h) {
                        break;
                    }
                    j -= h;
                } while (1);
                memcpy(Q_ADDR(base, j, size), mem, size);
            } /* for */
            h = (h -1) / step;
        } /* while (1 <= h) */
    } while (0);
    if (mem) {
        free(mem);
    }
}
