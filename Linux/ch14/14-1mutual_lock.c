#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int a = 0;
int b = 0;
pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;

void *another(void *arg)
{
    pthread_mutex_lock(&mutex_b);
    printf("in child thread, got mutex b, waiting for mutex a\n");
    sleep(5); // // 模拟 lock 时间差，保证子线程占有 mutex_b
    ++b;
    pthread_mutex_lock(&mutex_a); // 子线程等待 mutex_a
    b += a++;
    printf("child thread: %d %d\n", a, b);
    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);
    pthread_exit(NULL);
}

int main()
{
    pthread_t id;

    pthread_mutex_init(&mutex_a, NULL);
    pthread_mutex_init(&mutex_b, NULL);
    pthread_create(&id, NULL, another, NULL);

    pthread_mutex_lock(&mutex_a);
    printf("in parent thread, got mutex a, waiting for mutex b\n");
    sleep(5); // 模拟 lock 时间差，保证主线程占有 mutex_a
    ++a;
    pthread_mutex_lock(&mutex_b); // 主线程等待 mutex_b
    a += b++;
    printf("main thread: %d %d\n", a, b);
    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);

    pthread_join(id, NULL);
    pthread_mutex_destroy(&mutex_a);
    pthread_mutex_destroy(&mutex_b);
    return 0;
}
