/*
    生产者消费者模型（粗略的版本）
    性能有点问题：
    1. 如果 producer 生产数据满之后会一直 while 循环消耗 CPU（当这个版本是 list 不会满）
    2. 如果 customer 没有数据会一直 while 循坏消耗 CPU
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// 创建一个互斥量
pthread_mutex_t mutex;

struct Node{
    int num;
    struct Node *next;
};

int num = 0;

// 头结点
struct Node * head = NULL;

void * producer(void * arg) {

    // 不断的创建新的节点，添加到链表中
    while(1) {
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = num ++;
        printf("add node, num : %d, tid : %ld\n", newNode->num, (unsigned long)pthread_self());
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }

    return NULL;
}

void * customer(void * arg) {

    while(1) {
        pthread_mutex_lock(&mutex);
        // 保存头结点的指针
        struct Node * tmp = head;

        // 判断是否有数据
        if(head != NULL) {
            // 有数据
            head = head->next;
            num = tmp->num;
            printf("del node, num : %d, tid : %ld\n", tmp->num, (unsigned long)pthread_self());
            free(tmp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
        } else {
            // 没有数据
            pthread_mutex_unlock(&mutex);
        }
    }
    return  NULL;
}

int main() {

    pthread_mutex_init(&mutex, NULL);

    // 创建5个生产者线程，和5个消费者线程
    pthread_t ptids[5], ctids[5];

    for(int i = 0; i < 5; i++) {
        pthread_create(&ptids[i], NULL, producer, NULL);
        pthread_create(&ctids[i], NULL, customer, NULL);
    }

    // 阻塞回收子线程资源
    for(int i = 0; i < 5; i++) {
        pthread_join(ptids[i], NULL);
        pthread_join(ctids[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);

    return 0;
}