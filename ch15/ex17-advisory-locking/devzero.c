#include "apue.h"
#include "common.h"
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define NLOOPS      1000
#define SIZE        sizeof(long)    /* size of shared memory area */
#define SHM_MODE    0600

#define PARENT  0
#define CHILD   1


static int
update(long *ptr)
{
    fprintf(stderr, "Increasing %ld\n", *ptr);
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

    fd = open(argv[0], O_RDONLY);
    // Parent holds both locks at the beggining.
    writew_lock(fd, PARENT, SEEK_SET, 1);
    writew_lock(fd, CHILD, SEEK_SET, 1);


    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {           /* parent */
        for (i = 0; i < NLOOPS; i += 2) {

            // Can parent run?
            writew_lock(fd, PARENT, SEEK_SET, 1);

            if ((counter = update((long *)area)) != i)
                err_quit("parent: expected %d, got %d", i, counter);

            // Allow child to run.
            un_lock(fd, CHILD, SEEK_SET, 1);
        }
    } else {                        /* child */
        for (i = 1; i < NLOOPS + 1; i += 2) {
            // Can child run?
            writew_lock(fd, CHILD, SEEK_SET, 1);

            if ((counter = update((long *)area)) != i)
                err_quit("child: expected %d, got %d", i, counter);

            // Allow parent to run.
            un_lock(fd, PARENT, SEEK_SET, 1);
        }
    }

    exit(0);
}
