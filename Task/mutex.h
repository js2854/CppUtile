#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>

class Mutex
{
public:
    Mutex(void) { pthread_mutex_init(&m_lock, NULL); }
    virtual ~Mutex(void) { pthread_mutex_destroy(&m_lock); }
    void lock() { pthread_mutex_lock(&m_lock); }
    void unlock() { pthread_mutex_unlock(&m_lock); }
private:
    pthread_mutex_t m_lock;
};

#endif
