#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sin_type.h"
#include "sin_errno.h"
#include "sin_wi_queue.h"

struct sin_wi_queue
{
    struct sin_type t;
    struct sin_type_linkable *head;
    struct sin_type_linkable *tail;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    int length;
    char *name;
    int qlen;
};

struct sin_wi_queue *
sin_wi_queue_ctor(int qlen, int *e, const char *fmt, ...)
{
    struct sin_wi_queue *queue;
    va_list ap;
    int eval;

    queue = malloc(sizeof(*queue));
    if (queue == NULL) {
        _SET_ERR(e, ENOMEM);
        return (NULL);
    }
    memset(queue, '\0', sizeof(*queue));
    SIN_TYPE_SET(queue, _SIN_TYPE_WI_QUEUE);
    queue->qlen = qlen;
    if ((eval = pthread_cond_init(&queue->cond, NULL)) != 0) {
        free(queue);
        _SET_ERR(e, eval);
        return (NULL);
    }
    eval = pthread_mutex_init(&queue->mutex, NULL);
    if (eval != 0) {
        pthread_cond_destroy(&queue->cond);
        free(queue);
        _SET_ERR(e, eval);
        return (NULL);
    }
    va_start(ap, fmt);
    vasprintf(&queue->name, fmt, ap);
    va_end(ap);
    if (queue->name == NULL) {
        pthread_cond_destroy(&queue->cond);
        pthread_mutex_destroy(&queue->mutex);
        free(queue);
        _SET_ERR(e, ENOMEM);
        return (NULL);
    }
    return (queue);
}

void
sin_wi_queue_dtor(struct sin_wi_queue *queue)
{

    SIN_TYPE_ASSERT(queue, _SIN_TYPE_WI_QUEUE);
    pthread_cond_destroy(&queue->cond);
    pthread_mutex_destroy(&queue->mutex);
    free(queue->name);
    free(queue);
}

void
sin_wi_queue_put_item(void *wi, struct sin_wi_queue *queue)
{
    struct sin_type_linkable *stlp;

    stlp = (struct sin_type_linkable *)wi;

    pthread_mutex_lock(&queue->mutex);

    stlp->sin_next = NULL;
    if (queue->head == NULL) {
        queue->head = stlp;
        queue->tail = stlp;
    } else {
        queue->tail->sin_next = stlp;
        queue->tail = stlp;
    }
    queue->length += 1;
#if 0
    if (queue->length > 99 && queue->length % 100 == 0)
        fprintf(stderr, "queue(%s): length %d\n", queue->name, queue->length);
#endif

    if (queue->length % queue->qlen == 0) {
        /* notify worker thread */
        pthread_cond_signal(&queue->cond);
    }

    pthread_mutex_unlock(&queue->mutex);
}

void *
sin_wi_queue_get_item(struct sin_wi_queue *queue, int waitok,
  int return_on_wake)
{
    struct sin_type_linkable *wi;

    pthread_mutex_lock(&queue->mutex);
    while (queue->head == NULL) {
        if (waitok == 0) {
            pthread_mutex_unlock(&queue->mutex);
            return (NULL);
        }
        pthread_cond_wait(&queue->cond, &queue->mutex);
        if (queue->head == NULL && return_on_wake != 0) {
            pthread_mutex_unlock(&queue->mutex);
            return (NULL);
        }
    }
    wi = queue->head;
    queue->head = wi->sin_next;
    if (queue->head == NULL)
        queue->tail = NULL;
    queue->length -= 1;
    pthread_mutex_unlock(&queue->mutex);

    return (wi);
}