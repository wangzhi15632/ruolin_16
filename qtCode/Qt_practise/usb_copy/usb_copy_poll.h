#ifndef __USB_COPY_POLL_H__
#define __USB_COPY_POLL_H__
#include <pthread.h>

#define USB_COPY_MAX_THREAD	16
#define USB_COPY_DIR_EMPTY_TIME 10

typedef struct threadpool
{
	condition_t ready;
	task_t *first;
	task_t *last;
	int counter;
	int idle;
	int max_threads;
	int quit;
}threadpool_t;

#ifdef __cplusplus
extern "C" {
#endif
extern void threadpool_init(threadpool_t *pool, int threads);

#ifdef __cplusplus
}
#endif

#endif

