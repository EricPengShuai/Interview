/*
    一般情况下,main函数所在的线程我们称之为主线程（main线程），其余创建的线程
    称之为子线程。
    程序中默认只有一个进程，fork()函数调用，2个进程
    程序中默认只有一个线程，pthread_create()函数调用，2个线程。

    #include <pthread.h>
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
    void *(*start_routine) (void *), void *arg);

        - 功能：创建一个子线程
        - 参数：
            - thread：传出参数，线程创建成功后，子线程的线程ID被写到该变量中。
            - attr : 设置线程的属性，一般使用默认值，NULL
            - start_routine : 函数指针，这个函数是子线程需要处理的逻辑代码
            - arg : 给第三个参数使用，传参
        - 返回值：
            成功：0
            失败：返回错误号。这个错误号和之前errno不太一样。
            获取错误号的信息：  char * strerror(int errnum);

*/
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

// 子线程是可以修改 arg 的
void *callback(void *arg)
{
    printf("child thread...\n");
    printf("arg value: %d\n", *(int *)arg);
    *(int *)arg = 3;
    return NULL;
}

int main()
{
    pthread_t tid;
    int num = 10;
    
    // 创建一个子线程
    int ret = pthread_create(&tid, NULL, callback, (void *)&num);

    if (ret != 0) {
        char *errstr = strerror(ret);
        printf("error : %s\n", errstr);
    }

    for (int i = 0; i < 5; i++) {
        printf("%d\n", i);
    }

    sleep(1);

    // 子线程修改了 num
    printf("main thread: num = %d\n", num);
    return 0; // exit(0);
}