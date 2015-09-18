#include "apue.h"
#include <pthread.h>
#include <errno.h>

typedef struct my_barrier_t {
    pthread_mutex_t lock;
    pthread_cond_t  cond;
    unsigned int    count;
    unsigned int    finished;
} my_barrier_t;


int
my_barrier_wait(my_barrier_t *barrier) {
    int ret;
    unsigned long self = (unsigned long)pthread_self();
    printf("%lu: about to lock on &barrier->lock\n", self);
    pthread_mutex_lock(&barrier->lock);
    printf("%lu: locked\n", self);
    barrier->finished++;
    printf("%lu: count=%d, finished=%d\n", self, barrier->count, barrier->finished);
    if (barrier->count == barrier->finished) {
        pthread_cond_broadcast(&barrier->cond);
        printf("%lu: finished last, exiting\n", self);
        ret = PTHREAD_BARRIER_SERIAL_THREAD;
    }
    ret = pthread_cond_wait(&barrier->cond, &barrier->lock);
    printf("%lu: unblocked, unblocking & exiting\n", self);
    pthread_mutex_unlock(&barrier->lock);
    return ret;
}


int
my_barrier_destroy(my_barrier_t *barrier) {
    int err;
    printf("Destroying mutex\n");
    err = pthread_mutex_destroy(&barrier->lock);
    if (err != 0)
        return err;
    printf("Destroying condition\n");
    err = pthread_cond_destroy(&barrier->cond);
    if (err != 0)
        return err;
    return 0;
}


int
my_barrier_init(my_barrier_t *barrier, void *attr, unsigned count) {
    int err;
    pthread_mutexattr_t mutexattr;

    if (count < 1) {
        return EINVAL;
    }

    err = pthread_mutex_init(&barrier->lock, NULL);
    if (err != 0)
        return err;

    err = pthread_mutexattr_init(&mutexattr);
    if (err != 0)
        return err;
    err = pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);
    if (err != 0)
        return err;

    err = pthread_cond_init(&barrier->cond, NULL);
    if (err != 0)
        return err;

    barrier->count = count;
    barrier->finished = 0;
    return 0;
}


void *
thread(void *arg) {
    my_barrier_t *barrier_p = (my_barrier_t*)arg;
    printf("%lu: starting\n", (unsigned long)pthread_self());
    my_barrier_wait(barrier_p);
    printf("%lu: after barrier_wait\n", (unsigned long)pthread_self());
    return NULL;
}


#define NTHREADS 3


int main() {
    int err;
    pthread_t threads[NTHREADS];

    my_barrier_t barrier;
    err = my_barrier_init(&barrier, NULL, NTHREADS + 1);
    if (err != 0) {
        err_exit(err, "Couldn't init barrier");
    }

    for (int i = 0; i < NTHREADS; ++i) {
        err = pthread_create(&threads[i], NULL, thread, &barrier);
        if (err != 0)
            err_exit(err, "can't create thread %d", i);
    }

    printf("Main thread (%lu): about to wait for barrier\n", (unsigned long)pthread_self());
    my_barrier_wait(&barrier);
    printf("Main thread (%lu): after wait for barrier\n", (unsigned long)pthread_self());

    /* sleep(5); */

    err = my_barrier_destroy(&barrier);
    if (err != 0) {
        err_exit(err, "Couldn't destroy barrier");
    }
}
