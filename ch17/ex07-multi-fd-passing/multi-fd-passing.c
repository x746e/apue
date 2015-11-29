#include "apue.h"
#include "common.h"
#include <assert.h>
#include <fcntl.h>
#include <sys/socket.h>

#define MAX_FDS_LEN 1024


void send_fds(int sockfd, int fds[], int fds_len) {
    struct msghdr   msg = { 0 };
    struct cmsghdr  *cmsg;
    int             fds_size = sizeof(int) * fds_len;
    char            buf[CMSG_SPACE(fds_size)];
    struct iovec    iov[1];
    int             iov_buf[1];

    assert(fds_len < MAX_FDS_LEN);
    iov_buf[0] = fds_len;

    iov[0].iov_base = iov_buf;
    iov[0].iov_len  = sizeof iov_buf;
    msg.msg_iov     = iov;
    msg.msg_iovlen  = 1;

    msg.msg_control = buf;
    msg.msg_controllen = sizeof buf;

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(fds_size);

    memcpy(CMSG_DATA(cmsg), fds, fds_size); 

    msg.msg_controllen = cmsg->cmsg_len;

    printf("my_send_fd: about to send fds:");
    for (int i = 0; i < fds_len; ++i) printf(" %d", fds[i]);
    printf("\n");
    sys_chk(sendmsg(sockfd, &msg, 0));
    printf("my_send_fd: send\n");
}


void recv_fds(int sockfd, int **fds, int *fds_len) {
    struct msghdr   msg = { 0 };
    struct cmsghdr  *cmsg = malloc(CMSG_LEN(sizeof(int) * MAX_FDS_LEN));
    if (cmsg == NULL)
        err_quit("malloc error");
    struct iovec    iov[1];
    int             iov_buf[1];

    iov[0].iov_base = iov_buf;
    iov[0].iov_len  = sizeof(iov_buf);
    msg.msg_iov     = iov;
    msg.msg_iovlen  = 1;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;

    msg.msg_control = cmsg;
    msg.msg_controllen = CMSG_LEN(sizeof(int) * MAX_FDS_LEN);

    printf("my_recv_fd: about to recieve fd\n");
    sys_chk(recvmsg(sockfd, &msg, 0));
    printf("my_recv_fd: recieved\n");
    cmsg = CMSG_FIRSTHDR(&msg);
    *fds = (int *)CMSG_DATA(cmsg);
    *fds_len = iov_buf[0];
}


int main() {
    int pid;
    int fds[3];
    int spair[2];

    sys_chk(socketpair(AF_UNIX, SOCK_STREAM, 0, spair));

    sys_chk(pid = fork());
    if (pid == 0) {  // Child.
        sys_chk(fds[0] = open("/etc/passwd", O_RDONLY));
        sys_chk(lseek(fds[0], 5, SEEK_SET));
        sys_chk(fds[1] = open("/etc/group", O_RDONLY));
        sys_chk(lseek(fds[1], 10, SEEK_SET));
        sys_chk(fds[2] = open("/bin/sh", O_RDONLY));
        sys_chk(lseek(fds[2], 20, SEEK_SET));
        send_fds(spair[0], fds, 3);
        return EXIT_SUCCESS;
    } else {  // Parent.
        int *fds;
        int fds_len;
        recv_fds(spair[1], &fds, &fds_len);
        printf("parent: got fds:");
        for (int i = 0; i < fds_len; ++i) {
            printf(" %d (fp=%lld)", fds[i], (long long)lseek(fds[i], 0, SEEK_CUR));
        }
        printf("\n");
        return EXIT_SUCCESS;
    }
}
