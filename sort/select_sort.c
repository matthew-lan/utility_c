/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-18
*/


/*
 * 选择排序:
 *
 * 1. 初始时在序列中找到最小（大）元素，放到序列的起始位置作为已排序序列;
 * 2. 再从剩余未排序元素中继续寻找最小（大）元素，放到已排序序列的末尾;
 * 3. 以此类推，直到所有元素均排序完毕;
 */

/*
 * 选择排序与冒泡排序的区别:
 *
 * 1. 冒泡排序通过依次交换相邻两个顺序不合法的元素位置，从而将当前最小（大）元素放到合适的位置;
 * 2. 选择排序每遍历一次都记住了当前最小（大）元素的位置，最后仅需一次交换操作即可将其放到合适的位置;
 */

/*
 * 分类         :  内部比较排序
 * 数据结构      : 数组
 * 最差时间复杂度 : O(n^2)
 * 最优时间复杂度 : O(n^2)
 * 平均时间复杂度 : O(n^2)
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
/**
 * [select_sort description]
 * 选择排序.
 *
 * @param  base    [starting address of array]
 * @param  nmemb   [size of array members]
 * @param  size    [size of member]
 * @param  compar  [compare function]
 */
void select_sort(void *base, size_t nmemb, size_t size,
                 int (*compar)(const void *, const void *)
                 )
{
    for (size_t i = 0; i < nmemb -1; i++) {
        /* the position where to put */
        size_t t = i;

        /* find from i+1 to nmenb */
        for (size_t j = i +1; j < nmemb; j++) {
            /* !compar() */
            if (!compar(Q_ADDR(base, j, size), Q_ADDR(base, t, size))) {
                t = j;
            }
        }
        if (t != i) {
            Q_SWAP(Q_ADDR(base, t, size), Q_ADDR(base, i, size), size);
        }
    }
}
