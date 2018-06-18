/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-18
*/


/*
 * 重复地走访过要排序的元素，依次比较相邻两个元素，
 * 如果他们的顺序错误就把他们调换过来，直到没有元素再需要交换，排序完成;
 *
 * 这个算法的名字由来是因为越小(或越大)的元素会经由交换慢慢“浮”到数列的顶端;
 */

/*
 * 冒泡排序算法:
 *
 * 1. 比较相邻的元素，如果前一个比后一个大，就把它们两个调换位置;
 * 2. 对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对; 这步做完后，最后的元素会是最大的数;
 * 3. 针对所有的元素重复以上的步骤，除了最后一个;
 * 4. 持续每次对越来越少的元素重复上面的步骤，直到没有任何一对数字需要比较;
 */

/*
 * 分类         :  内部比较排序
 * 数据结构      : 数组
 * 最差时间复杂度 : O(n^2)
 * 最优时间复杂度 : 如果能在内部循环第一次运行时,使用一个旗标来表示有无需要交换的可能,可以把最优时间复杂度降低到O(n)
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
 * [bubble_sort description]
 * 冒泡排序.
 *
 * @param  base    [starting address of array]
 * @param  nmemb   [size of array members]
 * @param  size    [size of member]
 * @param  compar  [compare function]
 */
void bubble_sort(void *base, size_t nmemb, size_t size,
                 int (*compar)(const void *, const void *)
                 )
{
    /* the number of remaining needs to be sorted */
    for (size_t i = 0; i < nmemb -1; i++) {
        /* compare adjacent elements in turn */
        for (size_t j = 0; j < nmemb -1 - i; j++) {
            if (compar(Q_ADDR(base, j, size), Q_ADDR(base, j +1, size))) {
                Q_SWAP(Q_ADDR(base, j, size), Q_ADDR(base, j +1, size), size);
            }
        }
    }
}

