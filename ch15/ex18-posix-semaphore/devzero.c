#include "apue.h"
#include "common.h"
#include <stdbool.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <semaphore.h>

#define NLOOPS      1000
#define SIZE        sizeof(long)    /* size of shared memory area */
#define SHM_MODE    0600


static int
update(long *ptr)
{
    fprintf(stderr, "Increasing %ld\n", *ptr);
    return((*ptr)++);   /* return value before increment */
}

int
main(void)
{
    int     shmid, i, counter;
    void    *area;
    pid_t   pid;
    sem_t   *child_sem, *parent_sem;

    sys_chk(shmid = shmget(IPC_PRIVATE, SIZE, SHM_MODE));
    sys_chk((long)(area = shmat(shmid, 0, 0)));

    sys_sem_chk(parent_sem = sem_open("/parent", O_CREAT, S_IWUSR | S_IRUSR, 1));
    sys_sem_chk(child_sem = sem_open("/child", O_CREAT, S_IWUSR | S_IRUSR, 0));

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {           /* parent */
        for (i = 0; i < NLOOPS; i += 2) {
            // Can parent run?
            sys_chk(sem_wait(parent_sem));

            if ((counter = update((long *)area)) != i)
                err_quit("parent: expected %d, got %d", i, counter);

            // Allow child to run.
            sys_chk(sem_post(child_sem));
        }
    } else {                        /* child */
        for (i = 1; i < NLOOPS + 1; i += 2) {
            // Can child run?
            sys_chk(sem_wait(child_sem));

            if ((counter = update((long *)area)) != i)
                err_quit("child: expected %d, got %d", i, counter);

            // Allow parent to run.
            sys_chk(sem_post(parent_sem));
        }

        // Remove semaphores.
        sys_chk(sem_close(parent_sem));
        sys_chk(sem_unlink("/parent"));
        sys_chk(sem_close(child_sem));
        sys_chk(sem_unlink("/child"));
    }

    exit(0);
}
