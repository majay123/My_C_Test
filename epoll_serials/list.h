/*
 * 
 * 　　┏┓　　　┏┓+ +
 * 　┏┛┻━━━┛┻┓ + +
 * 　┃　　　　　　　┃ 　
 * 　┃　　　━　　　┃ ++ + + +
 *  ████━████ ┃+
 * 　┃　　　　　　　┃ +
 * 　┃　　　┻　　　┃
 * 　┃　　　　　　　┃ + +
 * 　┗━┓　　　┏━┛
 * 　　　┃　　　┃　　　　　　　　　　　
 * 　　　┃　　　┃ + + + +
 * 　　　┃　　　┃
 * 　　　┃　　　┃ +  神兽保佑
 * 　　　┃　　　┃    代码无bug　　
 * 　　　┃　　　┃　　+　　　　　　　　　
 * 　　　┃　 　　┗━━━┓ + +
 * 　　　┃ 　　　　　　　┣┓
 * 　　　┃ 　　　　　　　┏┛
 * 　　　┗┓┓┏━┳┓┏┛ + + + +
 * 　　　　┃┫┫　┃┫┫
 * 　　　　┗┻┛　┗┻┛+ + + +
 * 
 * 
 * 
 * ************Copyright 2022 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2022-02-24 10:26:58
 * @LastEditors  : MCD
 * @LastEditTime : 2022-03-01 09:50:33
 * @FilePath     : /My_C_Test/epoll_serials/list.h
 * @Description  : 
 * 
 * ******************************************
 */

#ifndef LISH_H
#define LISH_H

#ifndef NULL
#define NULL (0)
#endif

typedef struct list_head {
    struct list_head *prev, *next;
} list_head;

// 初始化
#define INIT_LIST_HEAD(ptr)                               \
    do                                                    \
    {                                                     \
        struct list_head *_ptr = (struct list_head *)ptr; \
        (_ptr)->next = (_ptr);                           \
        (_ptr->prev) = (_ptr);                            \
    }                                                     \
    while (0)

#define offsetof(TYPE, MEMBER) ((size_t) & ((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({             \
    const typeof(((type *)0)->member) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); \
})

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

// 插入新节点
static inline void __list_add(struct list_head *_new, struct list_head *prev, struct list_head *next)
{
    _new->next = next;
    next->prev = _new;
    prev->next = _new;
    _new->prev = prev;
}

// 头部新增
static inline void list_add(struct list_head *_new, struct list_head *head)
{
    __list_add(_new, head, head->next);
}

// 尾部增加
static inline void list_add_tail(struct list_head *_new, struct list_head *head)
{
    __list_add(_new, head->prev, head);
}

// 删除节点
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    prev->next = next;
    next->prev = prev;
}

// 删除entry节点
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

// 链表判断空
static inline int list_empty(struct list_head *head)
{
    return (head->next == head) && (head->prev == head);
}
#endif // !LISH_H