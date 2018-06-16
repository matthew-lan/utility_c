/*
* @Author: matthew
* @Date:   2018-05-19
* @Last Modified by:   matthew
* @Last Modified time: 2018-05-19
*/

#ifndef Q_LIST_H
#define Q_LIST_H

/**********************************************************
 * Includes
**********************************************************/
#include <stddef.h>


/**********************************************************
 * Types
**********************************************************/
struct q_list_head
{
    struct q_list_head *prev;
    struct q_list_head *next;
};


/**********************************************************
 * Functions
**********************************************************/
/* create a static list_head */
#define Q_LIST_HEAD(name) \
        struct q_list_head name = { &(name), &(name) }


/* initialize a list_head */
static inline void q_init_list_head(struct q_list_head *list)
{
    list->prev = list;
    list->next = list;
}


/* insert node between prev and next */
static inline void __q_list_add(struct q_list_head *node,
                                struct q_list_head *prev,
                                struct q_list_head *next
                                )
{
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

/* add head before node */
static inline void q_list_add(struct q_list_head *node,
                              struct q_list_head *head
                              )
{
    __q_list_add(node, head, head->next);
}

/* add head after node */
static inline void q_list_add_tail(struct q_list_head *node,
                                   struct q_list_head *head
                                   )
{
    __q_list_add(node, head->prev, head);
}


/* del the node between prev and next */
static inline void __q_list_del(struct q_list_head *prev,
                                struct q_list_head *next
                                )
{
    next->prev = prev;
    prev->next = next;
}

/* del the entry node */
static inline void q_list_del(struct q_list_head *entry)
{
    __q_list_del(entry->prev, entry->next);
    entry->prev = NULL;
    entry->next = NULL;
}


/* move entry before head */
static inline void q_list_move(struct q_list_head *entry,
                               struct q_list_head *head
                               )
{
    __q_list_del(entry->prev, entry->next);
    q_list_add(entry, head);
}

/* move entry after head */
static inline void q_list_move_tail(struct q_list_head *entry,
                                    struct q_list_head *head
                                    )
{
    __q_list_del(entry->prev, entry->next);
    q_list_add_tail(entry, head);
}


/*
 * entry : the node to test;
 * head  : the head of the list;
 */
static inline int q_list_is_last(const struct q_list_head *entry,
                                 const struct q_list_head *head
                                 )
{
    return entry->next == head;
}

/* head : the list to test */
static inline int q_list_is_empty(const struct q_list_head *head)
{
    return head->next == head;
}


/* get the member position in type struct */
#define q_offsetof(type, member) \
        (size_t)( &(((type *)0)->member) )

/**
 * get the type struct pointer for/according to this ptr;
 *
 * ptr    : the pointer, which is the type of member;
 * type   : the type of the ptr struct which is embedded in;
 * member : the name of the ptr struct within the type struct;
 */
#define q_container_of(ptr, type, member) \
        ({ \
            const typeof( (((type *)0)->member) ) *__mptr = (ptr); \
            (type *)( (char *)__mptr - q_offsetof(type, member) ); \
        })


/* get the struct pointer for this entry */
#define q_list_entry(ptr, type, member) \
        q_container_of(ptr, type, member)

/**
 * get the first element from a list;
 *
 * head :   the head of the list;
 */
#define q_list_first_entry(head, type, member) \
        q_list_entry((head)->next, type, member)

/**
 * iterate over a list
 *
 * pos  : the pointer to use as a loop cursor;
 * head : the head of the list;
 */
#define q_list_for_each(pos, head) \
        for (pos = (head)->next; pos != (head); pos = pos->next)


#endif // Q_LIST_H
