/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-18
*/


/*
 * 鸡尾酒排序:
 *
 * 1. 也叫定向冒泡排序, 冒泡排序的改进;
 * 2. 与冒泡排序的不同处在于, 从低到高, 然后从高到低; 冒泡排序则仅从低到高去比较序列里的每个元素;
 */

/*
 * 分类         :  内部比较排序
 * 数据结构      : 数组
 * 最差时间复杂度 : O(n^2)
 * 最优时间复杂度 : 如果序列在一开始已经大部分排序过的话,会接近O(n)
 * 平均时间复杂度 : O(n^2)
 * 所需辅助空间  : O(1)
 * 稳定性       : 稳定
 */

/**********************************************************
 * Includes
**********************************************************/
#include "sort.h"


/**********************************************************
 * Fucntions
**********************************************************/
/**
 * [cocktail_sort description]
 * 鸡尾酒排序.
 *
 * @param  base    [starting address of array]
 * @param  nmemb   [size of array members]
 * @param  size    [size of member]
 * @param  compar  [compare function]
 */
void cocktail_sort(void *base, size_t nmemb, size_t size,
                   int (*compar)(const void *, const void *)
                   )
{
    /* initialize boundary condition */
    size_t left  = 0;
    size_t right = nmemb -1;

    while (left < right) {
        /* In the first half of the wheel, put the max/min in the back */
        for (size_t i = left; i < right; i++) {
            if (compar(Q_ADDR(base, i, size), Q_ADDR(base, i +1, size))) {
                Q_SWAP(Q_ADDR(base, i, size), Q_ADDR(base, i +1, size), size);
            }
        }
        --right;

        /* In the second half of the wheel, put the min/max in front */
        for (size_t j = right; j > left; j--) {
            if (compar(Q_ADDR(base, j -1, size), Q_ADDR(base, j, size))) {
                Q_SWAP(Q_ADDR(base, j -1, size), Q_ADDR(base, j, size), size);
            }
        }
        ++left;
    }
}
