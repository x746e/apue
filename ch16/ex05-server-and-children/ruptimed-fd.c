#include "apue.h"
#include "common.h"
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

extern int initserver(int, const struct sockaddr *, socklen_t, int);

void sig_chld(int signum) {
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
}

void
serve(int sockfd)
{
    int     clfd;
    pid_t   pid;

    struct sigaction sa;
    sa.sa_handler = &sig_chld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sys_chk(sigaction(SIGCHLD, &sa, 0));

    set_cloexec(sockfd);
    for (;;) {
        if ((clfd = accept(sockfd, NULL, NULL)) < 0) {
            fprintf(stderr, "ruptimed: accept error: %s\n",
              strerror(errno));
            exit(1);
        }
        if ((pid = fork()) < 0) {
            fprintf(stderr, "ruptimed: fork error: %s\n",
              strerror(errno));
            exit(1);
        } else if (pid == 0) {  /* child */
            if (dup2(clfd, STDOUT_FILENO) != STDOUT_FILENO ||
              dup2(clfd, STDERR_FILENO) != STDERR_FILENO) {
                fprintf(stderr, "ruptimed: unexpected error\n");
                exit(1);
            }
            if (clfd != STDOUT_FILENO && clfd != STDERR_FILENO)
                close(clfd);
            execl("/usr/bin/uptime", "uptime", (char *)0);
            fprintf(stderr, "ruptimed: unexpected return from exec: %s\n",
              strerror(errno));
        } else {        /* parent */
            close(clfd);
        }
    }
}

int
main(int argc, char *argv[])
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int             sockfd, err, n;
    char            *host;

    if (argc != 1)
        err_quit("usage: ruptimed");
    if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
        n = HOST_NAME_MAX;  /* best guess */
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    if ((err = getaddrinfo(host, "12345", &hint, &ailist)) != 0) {
        fprintf(stderr, "ruptimed: getaddrinfo error: %s\n",
          gai_strerror(err));
        exit(1);
    }
    for (aip = ailist; aip != NULL; aip = aip->ai_next) {
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr,
          aip->ai_addrlen, QLEN)) >= 0) {
            serve(sockfd);
            exit(0);
        }
    }
    exit(1);
}
