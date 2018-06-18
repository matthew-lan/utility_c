/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-17
*/

#ifndef SORT_H
#define SORT_H

/*
 * 排序算法往往指的是内部排序算法，即数据记录在内存中进行排序;
 */

/*
 * 排序算法大体可分为两种：
 *
 * 比较排序:
 *    时间复杂度O(nlogn) ~ O(n^2);
 *    主要有：冒泡排序，选择排序，插入排序，归并排序，堆排序，快速排序等;
 *
 * 非比较排序:
 *    时间复杂度可以达到O(n);
 *    主要有：计数排序，基数排序，桶排序等;
 */

/*
 * 排序算法的稳定性:
 *
 * 如果Ai = Aj，排序前Ai在Aj之前，排序后Ai还在Aj之前，则称这种排序算法是稳定的;
 * 保证排序前后两个相等的数的相对顺序不变;
 */


/**********************************************************
 * Includes
**********************************************************/
#include "swap.h"


/**********************************************************
 * Fucntions
**********************************************************/
/* 冒泡排序 */
extern void bubble_sort(void *base, size_t nmemb, size_t size,
                        int (*compar)(const void *, const void *)
                        );

/* 鸡尾酒排序 */
extern void cocktail_sort(void *base, size_t nmemb, size_t size,
                          int (*compar)(const void *, const void *)
                          );

/* 选择排序 */
extern void select_sort(void *base, size_t nmemb, size_t size,
                        int (*compar)(const void *, const void *)
                        );

/* 插入排序 */
extern void insert_sort(void *base, size_t nmemb, size_t size,
                        int (*compar)(const void *, const void *)
                        );

/* 二分插入排序 */
extern void dichinsert_sort(void *base, size_t nmemb, size_t size,
                            int (*compar)(const void *, const void *)
                            );

/* 希尔排序 */
extern void shell_sort(void *base, size_t nmemb, size_t size,
                       int (*compar)(const void *, const void *)
                       );

/* 递归式归并排序 */
extern void merge_recur_sort(void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *)
                      );

/* 迭代式归并排序 */
extern void merge_iter_sort(void *base, size_t nmemb, size_t size,
                            int (*compar)(const void *, const void *)
                            );

/* 堆排序 */
extern void heap_sort(void *base, size_t nmemb, size_t size,
                      int (*compar)(const void *, const void *)
                      );


/* 快速排序 */
extern void quick_sort(void *base, size_t nmemb, size_t size,
                       int (*compar)(const void *, const void *)
                       );


#endif // SORT_H
