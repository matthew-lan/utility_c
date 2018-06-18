/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-18
*/


/*
 * 堆排序:
 *
 * 1. 由输入的无序数组构造一个最大堆，作为初始的无序区;
 * 2. 把堆顶元素（最大值）和堆尾元素互换;
 * 3. 把堆（无序区）的尺寸缩小1，并调用heapify(A, 0)从新的堆顶元素开始进行堆调整;
 * 4. 重复步骤2，直到堆的尺寸为1;
 */

/*
 * 分类         :  内部比较排序
 * 数据结构      : 数组
 * 最差时间复杂度 : O(nlogn)
 * 最优时间复杂度 : O(nlogn)
 * 平均时间复杂度 : O(nlogn)
 * 所需辅助空间  : O(1)
 * 稳定性       : 不稳定
 */

/**********************************************************
 * Includes
**********************************************************/
#include "sort.h"


/**********************************************************
 * Fucntions
**********************************************************/

static void heapify(void *base, size_t i, size_t nmemb, size_t size,
                    int (*compar)(const void *, const void *)
                    )
{
    size_t left  = 2* i +1;
    size_t right = 2* i +2;
    size_t max   = i;

    if ((left < nmemb) \
        && compar(Q_ADDR(base, left, size), Q_ADDR(base, max, size))) {
        max = left;
    }
    if ((right < nmemb) \
        && compar(Q_ADDR(base, right, size), Q_ADDR(base, max, size))) {
        max = right;
    }
    if (max != i) {
        Q_SWAP(Q_ADDR(base, i, size), Q_ADDR(base, max, size), size);
        heapify(base, max, nmemb, size, compar);
    }
}

static size_t heap_create(void *base, size_t nmemb, size_t size,
                         int (*compar)(const void *, const void *)
                         )
{
    size_t i = nmemb /2;
    do {
        if (!i) {
            break;
        }
        --i;
        heapify(base, i, nmemb, size, compar);
    } while (1);
    return nmemb;
}

/**
 * [insert_sort description]
 * 堆排序.
 *
 * @param  base    [starting address of array]
 * @param  nmemb   [size of array members]
 * @param  size    [size of member]
 * @param  compar  [compare function]
 */
void heap_sort(void *base, size_t nmemb, size_t size,
               int (*compar)(const void *, const void *)
               )
{
    size_t hn = heap_create(base, nmemb, size, compar);

    while (1 < hn) {
        --hn;
        Q_SWAP(Q_ADDR(base, 0, size), Q_ADDR(base, hn, size), size);
        heapify(base, 0, hn, size, compar);
    }
}
