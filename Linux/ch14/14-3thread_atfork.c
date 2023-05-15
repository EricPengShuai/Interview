#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
// #include <wait.h>

pthread_mutex_t mutex;

// 子线程运行的函数，首先获得互斥锁，然后暂停 5s，在释放互斥锁
void *another(void *arg)
{
    printf("in child thread, lock the mutex\n");
    pthread_mutex_lock(&mutex);
    sleep(5);
    pthread_mutex_unlock(&mutex);
}

void prepare()
{
    pthread_mutex_lock(&mutex);
}

void infork()
{
    pthread_mutex_unlock(&mutex);
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t id;
    pthread_create(&id, NULL, another, NULL);
    // prepare 句柄在 fork 调用创建出子进程之前被执行，lock 所有父进程的互斥锁
    // 第一个 infork 句柄在 fork 调用创建出子进程之后 fork 返回之前在父进程中执行，释放在 prepare 锁住的互斥锁
    // 第二个 infork 句柄在 fork 调用创建出子进程之后 fork 返回之前在子进程中执行，释放在 prepare 锁住的互斥锁
    pthread_atfork(prepare, infork, infork);

    // 在父进程中的主线程暂停 1s，以确保在执行 fork 操作之前，子线程已经开始运行并获得了互斥变量 mutex
    sleep(1);
    int pid = fork();
    if (pid < 0) // error
    {
        pthread_join(id, NULL);
        pthread_mutex_destroy(&mutex);
        return 1;
    }
    else if (pid == 0) // 子进程
    {
        printf("I am in the child, want to get the lock\n");
        // 子进程从父进程继承了互斥锁 mutex 的状态，该互斥锁处于锁住状态
        // 这是由于父进程中的子线程执行 pthread_mutex_lock 引起的，因此子进程中如果 lock 会一直阻塞
        pthread_mutex_lock(&mutex);
        printf("I can not run to here, oop...\n");
        pthread_mutex_unlock(&mutex);
        exit(0);
    }
    else // 父进程
    {
        pthread_mutex_unlock(&mutex);
        wait(NULL);
    }
    pthread_join(id, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
