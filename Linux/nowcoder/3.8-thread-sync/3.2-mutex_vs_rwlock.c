/*
此程序是为了比较
互斥锁和读写锁的在 高并发读操作上的效率差异
创建两个进程，父进程中使用互斥锁读取，子进程使用读写锁读取
每个进程中，创建10个读线程，对全局变量区的某个变量进行读取
每个线程读取 COUNT 次，总计完成 10*COUNT 次读取
利用 gettimeofday 函数，对整个读取过程精确到微秒的计时
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdlib.h>

// 创建互斥锁
pthread_mutex_t mutex;
// 创建读写锁
pthread_rwlock_t rwlock;
// 全局变量,父子进程、线程间都是相同的
int data = 123456;
// 读取次数
long COUNT = 100000;

// 互斥锁读函数
void *mFunc(void *arg)
{
    int read = 0;
    for (long i = 0; i < COUNT; ++i)
    {
        // 加锁
        pthread_mutex_lock(&mutex);
        read = data;
        // 解锁
        pthread_mutex_unlock(&mutex);
    }
    // 结束线程
    pthread_exit(NULL);
}

// 读写锁读函数
void *rwFunc(void *arg)
{
    int read = 0;
    for (long i = 0; i < COUNT; ++i)
    {
        // 加锁
        pthread_rwlock_rdlock(&rwlock);
        read = data;
        // 解锁
        pthread_rwlock_unlock(&rwlock);
    }
    // 结束线程
    pthread_exit(NULL);
}
int main()
{
    // 创建两个进程，父进程中使用互斥锁读取，子进程使用读写锁读取
    int pid = fork();
    if (pid > 0)
    {
        // parent process
        // 初始化互斥锁
        pthread_mutex_init(&mutex, NULL);
        // 创建十个线程，并开始计时
        pthread_t mtids[10];
        struct timeval start;
        gettimeofday(&start, NULL);
        for (int i = 0; i < 10; ++i)
        {
            pthread_create(&mtids[i], NULL, mFunc, NULL);
        }
        // 在主线程中，调用join函数，回收线程，线程回收完成后，结束计时
        for (int i = 0; i < 10; ++i)
        {
            pthread_join(mtids[i], NULL);
        }
        struct timeval end;
        gettimeofday(&end, NULL);
        long timediff = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        printf("互斥锁 读全部线程执行完毕，总耗时: %ld us\n", timediff);

        // 回收子进程
        wait(NULL);
    }
    else if (pid == 0)
    {
        // 子进程
        // 初始化读写锁
        pthread_rwlock_init(&rwlock, NULL);
        // 创建十个线程，并开始计时
        pthread_t rwtids[10];
        struct timeval start;
        gettimeofday(&start, NULL);
        for (int i = 0; i < 10; ++i)
        {
            pthread_create(&rwtids[i], NULL, rwFunc, NULL);
        }
        // 在主线程中，调用join函数，回收线程，线程回收完成后，结束计时
        for (int i = 0; i < 10; ++i)
        {
            pthread_join(rwtids[i], NULL);
        }
        struct timeval end;
        gettimeofday(&end, NULL);
        long timediff = (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
        printf("读写锁 读全部线程执行完毕，总耗时: %ld us\n", timediff);
        // 结束进程
        exit(0);
    }
    return 0;
}