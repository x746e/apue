#include "apue.h"
#include "common.h"
#include <sys/socket.h>
#include <fcntl.h>


int main() {
    int pid, fd;
    int fdpair[2];

    sys_chk(socketpair(AF_UNIX, SOCK_STREAM, 0, fdpair));

    TELL_WAIT();

    sys_chk(pid = fork());

    if (pid == 0) { // child
        sys_chk(fd = open("/etc/passwd", O_RDONLY));
        //printf("Child: sending fd: %d\n", fd);
        sys_chk(send_fd(fdpair[0], fd));

        sys_chk(lseek(fd, 10, SEEK_SET));
        TELL_PARENT(getppid());

        WAIT_PARENT();
        sys_chk(lseek(fd, 20, SEEK_SET));
        TELL_PARENT(getppid());

        return EXIT_SUCCESS;

    } else { // parent
        sys_chk(fd = recv_fd(fdpair[1], write));
        printf("Parent: got fd: %d, seek: %lld\n", fd, (long long)lseek(fd, 0, SEEK_CUR));

        WAIT_CHILD();
        printf("Parent: seek after child changed it: %lld (should be 10)\n", (long long)lseek(fd, 0, SEEK_CUR));
        TELL_CHILD(pid);

        WAIT_CHILD();
        printf("Parent: seek after child changed it second time: %lld (should be 20)\n", (long long)lseek(fd, 0, SEEK_CUR));

        return EXIT_SUCCESS;
    }

}
