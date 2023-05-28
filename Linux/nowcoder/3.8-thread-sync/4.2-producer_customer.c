#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <semaphore.h>

// 缓冲区
int *buf;
int bufSize = 100;
int bufPtr;
int count;
// 三个信号量
sem_t full, empty, mutex;

// 生产者线程
void *producer(void *arg)
{
    while (bufPtr < bufSize)
    {
        // 信号量模型，注意 mutex 必须在信号量 full 之后执行 P 操作
        sem_wait(&full);
        sem_wait(&mutex);
        ++ bufPtr;
        buf[bufPtr] = bufPtr;
        sem_post(&mutex);
        sem_post(&empty);
    }
    return NULL;
}

// 消费者线程
void *consumer(void *arg)
{
    while (1)
    {
        // 信号量模型
        sem_wait(&empty);
        sem_wait(&mutex);
        count = (count + 1) % __INT32_MAX__;
        printf("pid[%ld], count[%d], data[%d]\n", (unsigned long)pthread_self(), count, buf[bufPtr--]);

        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}
int main()
{
    // 初始化三个信号量
    sem_init(&full, 0, bufSize);
    sem_init(&empty, 0, 0);
    sem_init(&mutex, 0, 1); // 初始为 1 的信号量即为互斥量
    
    // 初始化读写指针、缓冲区
    bufPtr = -1;
    count = 0;
    buf = (int *)malloc(sizeof(int) * bufSize);
    
    // 创建6个线程，一个作生产者，5个消费者
    pthread_t ppid, cpids[5];
    pthread_create(&ppid, NULL, producer, NULL);
    for (int i = 0; i < 5; ++i) {
        pthread_create(&cpids[i], NULL, consumer, NULL);
    }

    // detach分离，线程自动回收资源
    pthread_detach(ppid);
    for (int i = 0; i < 5; ++i) {
        pthread_detach(cpids[i]);
    }
    // 主线程结束
    pthread_exit(NULL);
    return 0;
}