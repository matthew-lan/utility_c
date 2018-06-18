/*
* @Author: matthew.lan
* @Date:   2018-06-18
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-18
*/


/*
 * 快速排序:
 *
 * 使用分治策略(Divide and Conquer)来把一个序列分为两个子序列
 *
 * 1. 从序列中挑出一个元素，作为"基准"(pivot);
 * 2. 把所有比基准值小的元素放在基准前面，
 *    所有比基准值大的元素放在基准的后面（相同的数可以到任一边），
 *    这个称为分区(partition)操作;
 * 3. 对每个分区递归地进行步骤1~2，递归的结束条件是序列的大小是0或1，这时整体已经被排好序了;
 */

/*
 * 分类         :  内部比较排序
 * 数据结构      : 数组
 * 最差时间复杂度 : 每次选取的基准都是最大（或最小）的元素，导致每次只划分出了一个分区，
 *                需要进行n-1次划分才能结束递归，时间复杂度为O(n^2)
 *
 * 最优时间复杂度 : 每次选取的基准都是中位数，这样每次都均匀的划分出两个分区，
 *                只需要logn次划分就能结束递归，时间复杂度为O(nlogn)
 *
 * 平均时间复杂度 : O(nlogn)
 * 所需辅助空间  : 主要是递归造成的栈空间的使用(用来保存left和right等局部变量)，
 *               取决于递归树的深度，一般为O(logn)，最差为O(n)
 *
 * 稳定性       : 不稳定
 */

/**********************************************************
 * Includes
**********************************************************/
#include "sort.h"


/**********************************************************
 * Fucntions
**********************************************************/

static size_t __qs_partition(void *base, size_t left, size_t right, size_t size,
                             int (*compar)(const void *, const void *)
                             )
{
    /* select the last element as pivot every time */
    size_t pivot = right;
    /* the index of the last element of a subarray less than the benchmark */
    size_t tail  = left -1;

    /* list_for_each other elements except for the pivot */
    for (size_t i = left; i < right; i++) {
        /* put the element at the end of the previout subarray */
        if (!compar(Q_ADDR(base, i, size), Q_ADDR(base, pivot, size))) {
            ++tail;
            Q_SWAP(Q_ADDR(base, tail, size), Q_ADDR(base, i, size), size);
        }
    }
    /* put the pivot elemet at the end of the previout subarray */
    ++tail;
    Q_SWAP(Q_ADDR(base, tail, size), Q_ADDR(base, right, size), size);
    /* return the index of pivot */
    return tail;
}

static void __quick_sort(void *base, size_t left, size_t right, size_t size,
                         int (*compar)(const void *, const void *)
                         )
{
    if (left < right) {
        /* get the pivot index */
        size_t pivot_index = __qs_partition(base, left, right, size, compar);
        __quick_sort(base, left, pivot_index -1, size, compar);
        __quick_sort(base, pivot_index +1, right, size, compar);
    }
}

/**
 * [quick_sort description]
 * 快速排序.
 *
 * @param  base    [starting address of array]
 * @param  nmemb   [size of array members]
 * @param  size    [size of member]
 * @param  compar  [compare function]
 */
void quick_sort(void *base, size_t nmemb, size_t size,
                int (*compar)(const void *, const void *)
                )
{
    __quick_sort(base, 0, nmemb -1, size, compar);
}
