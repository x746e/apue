#include "apue.h"
#include "common.h"
#include <poll.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/socket.h>

#define NQ      3       /* number of queues */
#define MAXMSZ  512     /* maximum message size */
#define KEY     0x123   /* key for first message queue */

struct threadinfo {
    int qid;
    int fd;
};

struct mymesg {
    long mtype;
    char mtext[MAXMSZ];
};

char *buf = "hello";

void *
helper(void *arg)
{
    int                 n;
    struct mymesg       *m;
    struct threadinfo   *tip = arg;

    char *ptr;

    for(;;) {
        m = malloc(sizeof(struct mymesg));
        memset(m, 0, sizeof(struct mymesg));

        if ((n = msgrcv(tip->qid, m, MAXMSZ, 0, MSG_NOERROR)) < 0)
            err_sys("msgrcv error");

        ptr = m->mtext;
        /* ptr = buf; */

        printf("ptr: %p\n", ptr);
        printf("m->mtext: %p\n", m->mtext);
        printf("&ptr: %p\n", &ptr);
        printf("&(m->mtext): %p\n", &(m->mtext));

        fprintf(stderr, "Sending pointer: %p\n", ptr);
        if (write(tip->fd, &ptr, sizeof(char*)) < 0)
            err_sys("write error");
    }

}

int main() {
    int                 i, n, err;
    int                 fd[2];
    int                 qid[NQ];
    struct pollfd       pfd[NQ];
    struct threadinfo   ti[NQ];
    pthread_t           tid[NQ];
    char                *buf;
    int                 read_fd, write_fd;

        i = 0;

        if ((qid[i] = msgget((KEY+i), IPC_CREAT|0666)) < 0)
            err_sys("msgget error");

        sys_chk(pipe(fd));
        read_fd = fd[0];
        write_fd = fd[1];

        ti[i].qid = qid[i];
        ti[i].fd = write_fd;

        if ((err = pthread_create(&tid[i], NULL, helper, &ti[i]) != 0))
            err_exit(err, "pthread_create error");


    char *buf1;


    printf("buf1 before read: %p\n", buf1);
    sys_chk(read(read_fd, &buf1, sizeof(char*)));
    printf("Got: addr: %p\n", buf1);
    printf("     text: '%s' \n", buf1);

    sleep(1);
    return EXIT_SUCCESS;
}
