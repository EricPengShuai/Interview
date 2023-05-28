/*
    信号量的类型 sem_t
    int sem_init(sem_t *sem, int pshared, unsigned int value);
        - 初始化信号量
        - 参数：
            - sem : 信号量变量的地址
            - pshared : 0 用在线程间 ，非0 用在进程间
            - value : 信号量中的值

    int sem_destroy(sem_t *sem);
        - 释放资源

    int sem_wait(sem_t *sem);
        - 对信号量加锁，调用一次对信号量的值-1，如果值为0，就阻塞

    int sem_trywait(sem_t *sem);

    int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
    int sem_post(sem_t *sem);
        - 对信号量解锁，调用一次对信号量的值+1

    int sem_getvalue(sem_t *sem, int *sval);

    sem_t psem;
    sem_t csem;
    init(psem, 0, 8);
    init(csem, 0, 0);

    producer() {
        sem_wait(&psem);
        sem_post(&csem)
    }

    customer() {
        sem_wait(&csem);
        sem_post(&psem)
    }

*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

int val = 0;

// 创建一个互斥量
pthread_mutex_t mutex;
// 创建两个信号量
sem_t* psem;
sem_t* csem;

struct Node{
    int num;
    struct Node *next;
};

// 头结点
struct Node * head = NULL;

void * producer(void * arg) {
    // 不断的创建新的节点，添加到链表中
    while(1) {
        sem_wait(psem);

        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node *)malloc(sizeof(struct Node));
        newNode->next = head;
        head = newNode;
        newNode->num = val ++;
        printf("add node, num : %d, tid : %ld\n", newNode->num, (unsigned long)pthread_self());
        pthread_mutex_unlock(&mutex);

        sem_post(csem);
    }

    return NULL;
}

void * customer(void * arg) {
    while(1) {
        sem_wait(csem);

        pthread_mutex_lock(&mutex);
        // 保存头结点的指针
        struct Node * tmp = head;
        if (!head) {
            printf("head is nullptr\n");
            // pthread_exit(NULL);
            exit(0);
        }
        head = head->next;
        val = tmp->num;
        printf("del node, num : %d, tid : %ld\n", tmp->num, (unsigned long)pthread_self());
        free(tmp);
        pthread_mutex_unlock(&mutex);
        
        sem_post(psem);
    }
    return  NULL;
}

int main() {

    pthread_mutex_init(&mutex, NULL);

    // sem_init(psem, 0, 8);
    // sem_init(csem, 0, 0);
    psem = sem_open("/psem", O_CREAT, 0664, 8);
    if (psem == SEM_FAILED) {
        perror("sem_open");
        exit(0);
    }
    csem = sem_open("/csem", O_CREAT, 0664, 0);
    if (csem == SEM_FAILED) {
        perror("sem_open");
        exit(0);
    }

    // 创建5个生产者线程，和5个消费者线程
    pthread_t ptids[5], ctids[5];

    for(int i = 0; i < 5; i++) {
        pthread_create(&ptids[i], NULL, producer, NULL);
        pthread_create(&ctids[i], NULL, customer, NULL);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(ptids[i], NULL);
        pthread_join(ctids[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    // sem_destroy(psem);
    // sem_destroy(csem);
    
    // sem_close(psem);
    // sem_close(csem);
    sem_unlink("/psem");
    sem_unlink("/csem");

    pthread_exit(NULL);

    return 0;
}
