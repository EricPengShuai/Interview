#ifndef LST_TIMER
#define LST_TIMER

#include <time.h>
#include <stdio.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 64
class util_timer; // 前向声明
// 用户数据结构：客户端 socket 地址、socket 文件描述符、读缓存和定时器
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[BUFFER_SIZE];
    util_timer *timer;
};

// 定时器类
class util_timer
{
public:
    util_timer() : prev(NULL), next(NULL) {}

public:
    time_t expire;  // 任务超时时间，这里使用绝对时间
    void (*cb_func)(client_data *); // 任务的回调函数
    client_data *user_data; // 回调函数处理的客户数据，由定时器的执行者传递给回调函数
    
    util_timer *prev; // 双向链表指针
    util_timer *next;
};

// 定时器链表，它是一个升序、双向链表，且带有头结点和尾节点
class sort_timer_lst
{
public:
    sort_timer_lst() : head(NULL), tail(NULL) {}
    
    ~sort_timer_lst()   // 链表被销毁时，删除其中所有的定时器
    {
        util_timer *tmp = head;
        while (tmp)
        {
            head = tmp->next;
            delete tmp;
            tmp = head;
        }
    }

    // 将目标定时器 timer 添加到链表中
    void add_timer(util_timer *timer)
    {
        if (!timer) {
            return;
        }
        if (!head) {
            head = tail = timer;
            return;
        }
        // 如果目标定时器的超时时间小于当前链表中所有定时器的超时时间，则将该定时器插入头部，作为新链表的头节点
        if (timer->expire < head->expire) {
            timer->next = head;
            head->prev = timer;
            head = timer;
            return;
        }
        // 否则就需要调用重载函数，将它插入链表中合适的位置以保证链表的升序特征
        add_timer(timer, head);
    }

    // 当某个定时任务发生变化时，调整对应的定时器在链表中的位置（只考虑被调整的定时器超时时间延长的情况）
    void adjust_timer(util_timer *timer)
    {
        if (!timer) {
            return;
        }
        util_timer *tmp = timer->next;
        // 如果被调整的目标定时器处在链表尾部，或者该定时器新的超时值仍然小于下一个定时器的超时值，则不用调整
        if (!tmp || (timer->expire < tmp->expire)) {
            return;
        }
        // 如果目标定时器是链表的头节点，则将该定时器从链表中取出并重新插入链表
        if (timer == head) {
            head = head->next;
            head->prev = NULL;
            timer->next = NULL;
            add_timer(timer, head);
        } else { // 不是头节点直接取出，然后插入所在位置后续链表中
            timer->prev->next = timer->next;
            timer->next->prev = timer->prev;
            add_timer(timer, timer->next);
        }
    }

    // 将目标定时器 timer 从链表中删除
    void del_timer(util_timer *timer)
    {
        if (!timer) {
            return;
        }
        // 只有一个定时器，即目标定时器
        if ((timer == head) && (timer == tail)) {
            delete timer;
            head = NULL;
            tail = NULL;
            return;
        }

        if (timer == head) {    // 删除头节点
            head = head->next;
            head->prev = NULL;
            delete timer;
            return;
        }
        if (timer == tail) {    // 删除尾节点
            tail = tail->prev;
            tail->next = NULL;
            delete timer;
            return;
        }
        // 目标定时器位于链表中间
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
        delete timer;
    }
    
    // SIGALRM 信号每次被触发就在其信号处理函数中执行一次 tick 函数已处理链表上的到期任务
    void tick()
    {
        if (!head) {
            return;
        }
        printf("timer tick\n");
        time_t cur = time(NULL);    // 获取系统当前时间
        util_timer *tmp = head;
        /* 从头节点开始依次处理每个定时器，直到遇到一个尚未到期的定时器，这是核心逻辑 */
        while (tmp)
        {
            if (cur < tmp->expire) { // 绝对时间比较是否过期
                break;
            }
            // 调用定时器的回调函数，以执行定时任务
            tmp->cb_func(tmp->user_data);
            // 执行之后将其从链表中删除，并重置头节点
            head = tmp->next;
            if (head)
            {
                head->prev = NULL;
            }
            delete tmp;
            tmp = head;
        }
    }

private:
    // 将目标定时器 timer 添加到节点 lst_head 之后的部分链表中
    // 时间复杂度 O(n)
    void add_timer(util_timer *timer, util_timer *lst_head)
    {
        util_timer *prev = lst_head;
        util_timer *tmp = prev->next;
        while (tmp)
        {
            // 在链表中找到第一个超时时间大于 timer 的节点
            if (timer->expire < tmp->expire)
            {
                prev->next = timer;
                timer->next = tmp;
                tmp->prev = timer;
                timer->prev = prev;
                break;
            }
            prev = tmp;
            tmp = tmp->next;
        }
        // 如果遍历结束之后仍未找到插入位置，直接插入尾部
        if (!tmp)
        {
            prev->next = timer;
            timer->prev = prev;
            timer->next = NULL;
            tail = timer;
        }
    }

private:
    util_timer *head;
    util_timer *tail;
};

#endif
