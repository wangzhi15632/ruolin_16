#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include "usb_copy.h"
#include "usb_copy_poll.h"
#include "usb_main.h"
#include "mainwindow.h"
#include "searchthread.h"

int SearchThread::condition_init(condition_t *cond)
{
    int status;

    if((status = pthread_mutex_init(&cond->pmutex,nullptr)))
        return status;
    if((status = pthread_cond_init(&cond->pcond,nullptr)))
        return status;

    return 0;
}
int SearchThread::condition_lock(condition_t *cond)
{
    return pthread_mutex_lock(&cond->pmutex);
}
int SearchThread::condition_unlock(condition_t *cond)
{
    return pthread_mutex_unlock(&cond->pmutex);
}
int SearchThread::condition_wait(condition_t *cond)
{
    return pthread_cond_wait(&cond->pcond,&cond->pmutex);
}
int SearchThread::condition_timewait(condition_t *cond,const struct timespec *abstime)
{
    return pthread_cond_timedwait(&cond->pcond,&cond->pmutex,abstime);
}
int SearchThread::condition_signal(condition_t *cond)
{
    return pthread_cond_signal(&cond->pcond);
}
int SearchThread::condition_broadcast(condition_t *cond)
{
    return pthread_cond_broadcast(&cond->pcond);
}
int SearchThread::condition_destory(condition_t *cond)
{
    int status;
    if((status = pthread_mutex_destroy(&cond->pmutex)))
        return status;
    if((status = pthread_cond_destroy(&cond->pcond)))
        return status;
    return 0;
}

int SearchThread::wait_dir_info(char *dir)
{
    time_t cur_time, old_time;
    /*用10S时间判断目录是否为空，为空的话则退出线程*/
    time(&old_time);
    while(dir_is_empty(dir))
    {
        sleep(1);
        time(&cur_time);
        if((cur_time - old_time) > USB_COPY_DIR_EMPTY_TIME)
        {
            return FALSE;
        }
    }

    return TRUE;
}

void* thread_routine(void *arg)
{
    int timeout;
    struct timespec abstime;

    printf("thread 0x%0x is starting\n",(int)pthread_self());
    threadpool_t *pool = (threadpool_t *)arg;

    while(1)
    {
        timeout = 0;
        condition_lock(&pool->ready);
        pool->idle++;

        while(pool->first == nullptr && !pool->quit)
        {
            printf("thread 0x%0x is waiting\n",(int)pthread_self());
            clock_gettime(CLOCK_REALTIME, &abstime);
            abstime.tv_sec += 20;
            int status=condition_timewait(&pool->ready, &abstime);
            if(status == ETIMEDOUT)
            {
                printf("thread 0x%0x is wait timed out\n",(int)pthread_self());
                timeout = 1;
                break;
            }
        }

        pool->idle--;

        if(pool->first != nullptr)
        {
            task_t *t = pool->first;
            pool->first = t->next;

            condition_unlock(&pool->ready);
            t->run(t->arg);
            free(t);
            condition_lock(&pool->ready);
        }

        if(pool->quit && pool->first == nullptr)
        {
            pool->counter--;
            if(pool->counter == 0)
            {
                condition_signal(&pool->ready);
            }
            condition_unlock(&pool->ready);

            break;
        }

        if(timeout && pool->first ==nullptr)
        {
            pool->counter--;
            condition_unlock(&pool->ready);

            break;
        }
        condition_unlock(&pool->ready);
    }

    printf("thread 0x%0x is exiting\n",(int)pthread_self());
    return nullptr;
}


void SearchThread::threadpool_init(threadpool_t *pool, int threads)
{
    condition_init(&pool ->ready);
    pool->first = nullptr;
    pool->last = nullptr;
    pool->counter = 0;
    pool->idle = 0;
    pool->max_threads = threads;
    pool->quit = 0;
}


void SearchThread::threadpool_add_task(threadpool_t *pool, void *(*run)(void *arg), void *arg)
{
    task_t *newstask = (task_t *)malloc(sizeof(task_t));
    char *tmp = (char *)malloc(20);

    memcpy(tmp, arg, 20);
    newstask->run = run;
    newstask->arg = tmp;
    newstask->next = nullptr;
    condition_lock(&pool->ready);

    if(pool->first == nullptr)
    {
        pool->first = newstask;
    }
    else
        pool->last->next = newstask;

    pool->last = newstask;

    if(pool->idle > 0)
    {
        condition_signal(&pool->ready);
    }
    else if(pool->counter < pool->max_threads)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, thread_routine, pool);
        pool->counter++;

    }
    condition_unlock(&pool->ready);
}

void SearchThread::threadpool_destory(threadpool_t *pool)
{

    if(pool->quit)
    {
        return;
    }
    condition_lock(&pool->ready);
    pool->quit = 1;
    if(pool->counter > 0)
    {
        if(pool->idle > 0)
            condition_broadcast(&pool->ready);

        while(pool->counter > 0)
        {
            condition_wait(&pool->ready);
        }
    }

    condition_unlock(&pool->ready);
    condition_destory(&pool->ready);
}


void* SearchThread::mytask(void *arg)
{
    printf("thread 0x%0x is working on task %s\n",(int)pthread_self(),(char*)arg);
    /*虽然已经挂载，但是挂载目录是一个空目录*/
    if(!wait_dir_info((char *)arg))
        return nullptr;

    cp_task((char*)arg);

    free(arg);

    return nullptr;
}

