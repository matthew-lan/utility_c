/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-18
*/


/*
 * 归并排序:
 *
 * 归并排序算法主要依赖归并(Merge)操作, 归并操作指的是将两个已经排序的序列合并成一个序列的操作;
 *
 * 1. 申请空间，使其大小为两个已经排序序列之和，该空间用来存放合并后的序列;
 * 2. 设定两个指针，最初位置分别为两个已经排序序列的起始位置;
 * 3. 比较两个指针所指向的元素，选择相对小的元素放入到合并空间，并移动指针到下一位置;
 * 4. 重复步骤3直到某一指针到达序列尾;
 * 5. 将另一序列剩下的所有元素直接复制到合并序列尾;
 */

/*
 * 分类         :  内部比较排序
 * 数据结构      : 数组
 * 最差时间复杂度 : O(nlogn)
 * 最优时间复杂度 : O(nlogn)
 * 平均时间复杂度 : O(nlogn)
 * 所需辅助空间  : O(n)
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

static void merge(void *base, size_t left, size_t mid, size_t right, size_t size,
                  int (*compar)(const void *, const void *)
                  )
{
    size_t nmemb = right - left +1;
    size_t i     = left;        /* starting addr of first array */
    size_t j     = mid +1;      /* starting addr of second array */
    size_t index = 0;
    char   *p    = (char *)malloc(nmemb * size);

    do {
        if (!p) {
            break;
        }

        while ((i <= mid) && (j <= right)) {
            if (!compar(Q_ADDR(base, i, size), Q_ADDR(base, j, size))) {
                memcpy(Q_ADDR(p, index, size), Q_ADDR(base, i, size), size);
                ++i;    /* shif the pointer of first array */
            }
            else {
                memcpy(Q_ADDR(p, index, size), Q_ADDR(base, j, size), size);
                ++j;    /* shif the pointer of second array */
            }
            ++index;
        }
        while (i <= mid) {
            memcpy(Q_ADDR(p, index, size), Q_ADDR(base, i, size), size);
            ++index;
            ++i;
        }
        while (j <= right) {
            memcpy(Q_ADDR(p, index, size), Q_ADDR(base, j, size), size);
            ++index;
            ++j;
        }
        for (size_t k = 0; k < nmemb; k++) {
            memcpy(Q_ADDR(base, left, size), Q_ADDR(p, k, size), size);
            ++left;
        }
    } while (0);
    if (p) {
        free(p);
    }
}

static void __merge_recur_sort(void *base, size_t left, size_t right, size_t size,
                               int (*compar)(const void *, const void *)
                               )
{
    if (left < right) {
        size_t mid = (left + right) /2;
        __merge_recur_sort(base, left, mid, size, compar);
        __merge_recur_sort(base, mid +1, right, size, compar);
        merge(base, left, mid, right, size, compar);
    }
}


/**
 * [merge_recur_sort description]
 * 递归式归并排序.
 * 自顶向下
 *
 * @param  base    [starting address of array]
 * @param  nmemb   [size of array members]
 * @param  size    [size of member]
 * @param  compar  [compare function]
 */
void merge_recur_sort(void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *)
                      )
{
    __merge_recur_sort(base, 0, nmemb -1, size, compar);
}

/**
 * [merge_iter_sort description]
 * 迭代式归并排序.
 * 自底向上
 *
 * @param  base    [starting address of array]
 * @param  nmemb   [size of array members]
 * @param  size    [size of member]
 * @param  compar  [compare function]
 */
void merge_iter_sort(void *base, size_t nmemb, size_t size,
                     int (*compar)(const void *, const void *)
                     )
{
    size_t left  = 0;
    size_t mid   = 0;
    size_t right = 0;

    for (size_t i = 1; i < nmemb; i *= 2) {
        left = 0;
        while (left + i < nmemb) {
            mid   = left + i -1;
            right = mid + i < nmemb ? mid + i : nmemb -1;
            merge(base, left, mid, right, size, compar);
            left  = right +1;
        }
    }
}
