/*
* @Author: matthew.lan
* @Date:   2018-06-17
* @Last Modified by:   matthew.lan
* @Last Modified time: 2018-06-18
*/


#include "log/log.h"
#include "sort/sort.h"

#define TAG_TEST    "TEST"


#define Q_sort(__sort, type) \
        sort_##type(__sort)


typedef void (*__sort)(void *base, size_t nmemb, size_t size,
                       int (*compar)(const void *, const void *)
                       );


static void sort_func_type(__sort xxx_sort)
{
    if (bubble_sort == xxx_sort) {
        Q_DEBUG_LOG(TAG_TEST, "bubble_sort()");
    }
    else if (cocktail_sort == xxx_sort) {
        Q_DEBUG_LOG(TAG_TEST, "cocktail_sort()");
    }
    else if (select_sort == xxx_sort) {
        Q_DEBUG_LOG(TAG_TEST, "select_sort()");
    }
    else if (insert_sort == xxx_sort) {
        Q_DEBUG_LOG(TAG_TEST, "insert_sort()");
    }
    else if (dichinsert_sort == xxx_sort) {
        Q_DEBUG_LOG(TAG_TEST, "dichinsert_sort()");
    }
    else if (shell_sort == xxx_sort) {
        Q_DEBUG_LOG(TAG_TEST, "shell_sort()");
    }
    else if (merge_recur_sort == xxx_sort) {
        Q_DEBUG_LOG(TAG_TEST, "merge_recur_sort()");
    }
    else if (merge_iter_sort == xxx_sort) {
        Q_DEBUG_LOG(TAG_TEST, "merge_iter_sort()");
    }
    else if (heap_sort == xxx_sort) {
        Q_DEBUG_LOG(TAG_TEST, "heap_sort()");
    }
    else if (quick_sort == xxx_sort) {
        Q_DEBUG_LOG(TAG_TEST, "quick_sort()");
    }
}

/**********************************************************
 * int array
**********************************************************/
static inline int compar_int(const void *x, const void *y)
{
    return *(int *)x > *(int *)y ? 1 : 0;
}

static void sort_int(__sort xxx_sort)
{
    int    array[] = {9, 0, 8, 1, 7, 2, 6, 3, 5, 4};
    size_t nmemb   = sizeof(array) / sizeof(int);
    size_t size    = sizeof(int);

    sort_func_type(xxx_sort);

    xxx_sort(array, nmemb, size, compar_int);

    for (size_t n = 0; n < nmemb; n++) {
        Q_DEBUG_LOG(TAG_TEST, "[%d] %d", n, array[n]);
    }
}

/**********************************************************
 * double array
**********************************************************/
static inline int compar_double(const void *x, const void *y)
{
    return 1e-5 < *(double *)x - *(double *)y ? 1 : 0;
}

static void sort_double(__sort xxx_sort)
{
    double array[] = {9.9, 0.0, 8.8, 1.1, 7.7, 2.2, 6.6, 3.3, 5.5, 4.4};
    size_t nmemb   = sizeof(array) / sizeof(double);
    size_t size    = sizeof(double);

    sort_func_type(xxx_sort);

    xxx_sort(array, nmemb, size, compar_double);

    for (size_t n = 0; n < nmemb; n++) {
        Q_DEBUG_LOG(TAG_TEST, "[%d] %0.2f", n, array[n]);
    }
}

/**********************************************************
 * struct array
**********************************************************/
typedef struct
{
    char *name;
    int  age;
} test_struct;

static inline int compar_test_struct(const void *x, const void *y)
{
    return ((test_struct *)x)->age > ((test_struct *)y)->age ? 1 : 0;
}

static void sort_test_struct(__sort xxx_sort)
{
    test_struct array[] = { {"aa", 29}, {"bb", 20},
                            {"cc", 28}, {"dd", 21},
                            {"ee", 27}, {"ff", 22},
                            {"gg", 26}, {"hh", 23},
                            {"ii", 25}, {"jj", 24}
                           };
    size_t      nmemb   = sizeof(array) / sizeof(test_struct);
    size_t      size    = sizeof(test_struct);

    sort_func_type(xxx_sort);

    xxx_sort(array, nmemb, size, compar_test_struct);

    for (size_t n = 0; n < nmemb; n++) {
        Q_DEBUG_LOG(TAG_TEST, "[%d] %d, %s", n, array[n].age, array[n].name);
    }
}

/**********************************************************
 * test bubble_sort
**********************************************************/
static void test_bubble_sort(void)
{
    Q_sort(bubble_sort, int);
    Q_sort(bubble_sort, double);
    Q_sort(bubble_sort, test_struct);
}

/**********************************************************
 * test cocktail_sort
**********************************************************/
static void test_cocktail_sort(void)
{
    Q_sort(cocktail_sort, int);
    Q_sort(cocktail_sort, double);
    Q_sort(cocktail_sort, test_struct);
}

/**********************************************************
 * test select_sort
**********************************************************/
static void test_select_sort(void)
{
    Q_sort(select_sort, int);
    Q_sort(select_sort, double);
    Q_sort(select_sort, test_struct);
}

/**********************************************************
 * test insert_sort
**********************************************************/
static void test_insert_sort(void)
{
    Q_sort(insert_sort, int);
    Q_sort(insert_sort, double);
    Q_sort(insert_sort, test_struct);
}

/**********************************************************
 * test dichinsert_sort
**********************************************************/
static void test_dichinsert_sort(void)
{
    Q_sort(dichinsert_sort, int);
    Q_sort(dichinsert_sort, double);
    Q_sort(dichinsert_sort, test_struct);
}

/**********************************************************
 * test shell_sort
**********************************************************/
static void test_shell_sort(void)
{
    Q_sort(shell_sort, int);
    Q_sort(shell_sort, double);
    Q_sort(shell_sort, test_struct);
}

/**********************************************************
 * test merge_sort
**********************************************************/
static void test_merge_recur_sort(void)
{
    Q_sort(merge_recur_sort, int);
    Q_sort(merge_recur_sort, double);
    Q_sort(merge_recur_sort, test_struct);
}

static void test_merge_iter_sort(void)
{
    Q_sort(merge_iter_sort, int);
    Q_sort(merge_iter_sort, double);
    Q_sort(merge_iter_sort, test_struct);
}

/**********************************************************
 * test heap_sort
**********************************************************/
static void test_heap_sort(void)
{
    Q_sort(heap_sort, int);
    Q_sort(heap_sort, double);
    Q_sort(heap_sort, test_struct);
}

/**********************************************************
 * test quick_sort
**********************************************************/
static void test_quick_sort(void)
{
    Q_sort(quick_sort, int);
    Q_sort(quick_sort, double);
    Q_sort(quick_sort, test_struct);
}


int main(int argc, char const *argv[])
{
    log_tags_set(TAG_TEST);

    test_bubble_sort();
    test_cocktail_sort();
    test_select_sort();
    test_insert_sort();
    test_dichinsert_sort();
    test_shell_sort();
    test_merge_recur_sort();
    test_merge_iter_sort();
    test_heap_sort();
    test_quick_sort();

    return 0;
}
