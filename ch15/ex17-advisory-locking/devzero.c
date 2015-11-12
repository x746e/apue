#include "apue.h"
#include "common.h"
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define NLOOPS      1000
#define SIZE        sizeof(long)    /* size of shared memory area */
#define SHM_MODE    0600


static int
update(long *ptr)
{
    fprintf(stderr, "%d: Increasing %ld\n", getpid(), *ptr);
    return((*ptr)++);   /* return value before increment */
}

int
main(int argc, char *argv[])
{
    int     shmid, fd, i, counter;
    void    *area;
    pid_t   pid;

    sys_chk(shmid = shmget(IPC_PRIVATE, SIZE, SHM_MODE));
    sys_chk((long)(area = shmat(shmid, 0, 0)));

    sys_chk(fd = open("devzero.c", O_RDWR));

    sys_chk(writew_lock(fd, 0, SEEK_SET, 1));

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {           /* parent */
        for (i = 0; i < NLOOPS; i += 2) {
            sleep(1);
            printf("parent: before lock\n");
            writew_lock(fd, 0, SEEK_SET, 1);
            printf("parent: got lock\n");

            if ((counter = update((long *)area)) != i)
                err_quit("parent: expected %d, got %d", i, counter);

            un_lock(fd, 0, SEEK_SET, 1);
            printf("parent: unlock\n");
        }
    } else {                        /* child */
        for (i = 1; i < NLOOPS + 1; i += 2) {
            sleep(1);
            printf("child: before lock\n");
            writew_lock(fd, 0, SEEK_SET, 1);
            printf("child: got lock\n");

            if ((counter = update((long *)area)) != i)
                err_quit("child: expected %d, got %d", i, counter);

            un_lock(fd, 0, SEEK_SET, 1);
            printf("child: unlock\n");
        }
    }

    exit(0);
}
