/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-18
*/


/*
 * 插入排序:
 *
 * 1. 从第一个元素开始，该元素可以认为已经被排序;
 * 2. 取出下一个元素，在已经排序的元素序列中从后向前扫描;
 * 3. 如果该元素（已排序）大于新元素，将该元素移到下一位置;
 * 4. 重复步骤3，直到找到已排序的元素小于或者等于新元素的位置;
 * 5. 将新元素插入到该位置;
 * 6. 重复步骤2~5;
 */

/*
 * 分类         :  内部比较排序
 * 数据结构      : 数组
 * 最差时间复杂度 : 最坏情况为输入序列是降序排列的,此时时间复杂度O(n^2)
 * 最优时间复杂度 : 最好情况为输入序列是升序排列的,此时时间复杂度O(n)
 * 平均时间复杂度 : O(n^2)
 * 所需辅助空间  : O(1)
 * 稳定性       : 稳定
 */

/*
 * 插入排序不适合对于数据量比较大的排序应用;
 * 在STL的sort算法和stdlib的qsort算法中，都将插入排序作为快速排序的补充，用于少量元素的排序（通常为8个或以下）;
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
 * [insert_sort description]
 * 插入排序.
 *
 * @param  base    [starting address of array]
 * @param  nmemb   [size of array members]
 * @param  size    [size of member]
 * @param  compar  [compare function]
 */
void insert_sort(void *base, size_t nmemb, size_t size,
                 int (*compar)(const void *, const void *)
                 )
{
    char *mem = (char *)malloc(size);

    do {
        if (!mem) {
            break;
        }

        for (size_t i = 1; i < nmemb; i++) {
            size_t t = i;
            size_t j = i -1;

            /* save what will be compared */
            memcpy(mem, Q_ADDR(base, t, size), size);

            do {
                /* right shift all array left members */
                if (compar(Q_ADDR(base, j, size), mem)) {
                    memcpy(Q_ADDR(base, j +1, size), Q_ADDR(base, j, size), size);
                }
                /* fix the position value and exit */
                else {
                    ++j;
                    break;
                }
                /* exit */
                if (!j) {
                    break;
                }
                --j;
            } while (1);
            memcpy(Q_ADDR(base, j, size), mem, size);
        }
    } while (0);
    if (mem) {
        free(mem);
    }
}
