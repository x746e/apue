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
    pthread_mutex_lock(&barrier->lock);
    barrier->finished++;
    if (barrier->count == barrier->finished) {
        pthread_mutex_unlock(&barrier->lock);
        pthread_cond_broadcast(&barrier->cond);
        return PTHREAD_BARRIER_SERIAL_THREAD;
    } else {
        return pthread_cond_wait(&barrier->cond, &barrier->lock);
    }
}


int
my_barrier_destroy(my_barrier_t *barrier) {
    int err;
    err = pthread_mutex_destroy(&barrier->lock);
    if (err != 0)
        return err;
    err = pthread_cond_destroy(&barrier->cond);
    if (err != 0)
        return err;
    return 0;
}


int
my_barrier_init(
        my_barrier_t *barrier,
        void *attr, unsigned count) {
    int err;
    pthread_mutexattr_t mutexattr;

    if (count < 0) {
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

    printf("Main thread: about to wait for barrier\n");
    my_barrier_wait(&barrier);
    printf("Main thread: after wait for barrier\n");

    sleep(5);

    /* err = my_barrier_destroy(&barrier); */
    /* if (err != 0) { */
    /*     err_exit(err, "Couldn't destroy barrier"); */
    /* } */
}
