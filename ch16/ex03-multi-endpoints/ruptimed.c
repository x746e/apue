#include "apue.h"
#include "common.h"
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUFLEN  128
#define QLEN 10
#define MAX_ENDPOINTS 64

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

extern int initserver(int, const struct sockaddr *, socklen_t, int);

void
serve_many(int sockets[], int nsockets)
{
    int     clfd, maxsock = -1;
    FILE    *fp;
    char    buf[BUFLEN];
    fd_set  init_set, set;

    FD_ZERO(&init_set);
    for (int i = 0; i < nsockets; ++i) {
        set_cloexec(sockets[i]);
        FD_SET(sockets[i], &init_set);
        if (sockets[i] > maxsock) {
            maxsock = sockets[i];
        }
    }


    for (;;) {
        set = init_set;
        sys_chk(select(maxsock + 1, &set, NULL, NULL, NULL));
        for (int i = 0; i < nsockets; ++i) {
            if (FD_ISSET(sockets[i], &set)) {
                if ((clfd = accept(sockets[i], NULL, NULL)) < 0) {
                    syslog(LOG_ERR, "ruptimed: accept error: %s",
                        strerror(errno));
                    exit(1);
                }
                set_cloexec(clfd);
                if ((fp = popen("/usr/bin/uptime", "r")) == NULL) {
                    sprintf(buf, "error: %s\n", strerror(errno));
                    send(clfd, buf, strlen(buf), 0);
                } else {
                    while (fgets(buf, BUFLEN, fp) != NULL)
                        send(clfd, buf, strlen(buf), 0);
                    pclose(fp);
                }
                close(clfd);
            }
        }

    }
}

int
main(int argc, char *argv[])
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int             sockfd, err, n, nendpoints = 0;
    int             endpoints[MAX_ENDPOINTS];
    char            *host;

    if (argc != 1)
        err_quit("usage: ruptimed");
    if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
        n = HOST_NAME_MAX;  /* best guess */
    if ((host = malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");
    /* daemonize("ruptimed"); */
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    if ((err = getaddrinfo(host, "12345", &hint, &ailist)) != 0) {
        syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s",
          gai_strerror(err));
        fprintf(stderr, "ruptimed: getaddrinfo error: %s\n",
          gai_strerror(err));
        exit(1);
    }
    for (aip = ailist; aip != NULL; aip = aip->ai_next) {
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr,
          aip->ai_addrlen, QLEN)) >= 0) {
            endpoints[nendpoints++] = sockfd;
        }
    }
    if (nendpoints) {
        serve_many(endpoints, nendpoints);
        exit(0);
    } else {
        fprintf(stderr, "No endpoints to serve on\n");
        exit(1);
    }
}
