#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

// 封装信号量的类
class sem
{
public:
    sem()
    {
        // 创建并初始化信号量
        if (sem_init(&m_sem, 0, 0) != 0) {
            throw std::exception();
        }
    }
    ~sem()
    {
        sem_destroy(&m_sem); // 销毁信号量
    }
    bool wait()
    {
        return sem_wait(&m_sem) == 0; // 等待信号量，P
    }
    bool post()
    {
        return sem_post(&m_sem) == 0; // 增加信号量，V
    }

private:
    sem_t m_sem;
};

// 封装互斥锁的类
class locker
{
public:
    locker()
    {
        // 创建并初始化互斥锁
        if (pthread_mutex_init(&m_mutex, NULL) != 0) {
            throw std::exception();
        }
    }
    ~locker()
    {
        pthread_mutex_destroy(&m_mutex); // 销毁互斥锁
    }
    bool lock()
    {
        return pthread_mutex_lock(&m_mutex) == 0; // 获取互斥锁   
    }
    bool unlock()
    {
        return pthread_mutex_unlock(&m_mutex) == 0; // 释放互斥锁
    }

private:
    pthread_mutex_t m_mutex;
};

// 封装条件变量的类
class cond
{
public:
    cond() // 创建并初始化条件变量
    {
        if (pthread_mutex_init(&m_mutex, NULL) != 0) {
            throw std::exception();
        }
        if (pthread_cond_init(&m_cond, NULL) != 0) {
            // 构造函数一旦出现问题就立即释放已经成功分配的资源
            pthread_mutex_destroy(&m_mutex);
            throw std::exception();
        }
    }
    ~cond() // 销毁条件变量
    {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_cond);
    }
    bool wait() // 等待条件变量
    {
        int ret = 0;
        pthread_mutex_lock(&m_mutex); // 必须先加锁
        ret = pthread_cond_wait(&m_cond, &m_mutex);
        pthread_mutex_unlock(&m_mutex);
        return ret == 0;
    }
    bool signal() // 唤醒等待条件变量线程
    {
        return pthread_cond_signal(&m_cond) == 0;
    }

private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};

#endif
