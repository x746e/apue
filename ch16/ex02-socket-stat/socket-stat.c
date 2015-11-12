#include "apue.h"
#include "common.h"
#include <sys/socket.h>
#include <sys/un.h>


int main() {
    int s;
    char sockpath[] = "foo.socket";
    struct sockaddr_un un;

    unlink(sockpath);

    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, sockpath);
    sys_chk(s = socket(AF_UNIX, SOCK_STREAM, 0));
    sys_chk(bind(s, (struct sockaddr *)&un, sizeof(struct sockaddr_un)));

    struct stat buf;
    sys_chk(fstat(s, &buf));

    printf("ID of device containing file: %lld\n",       (long long)buf.st_dev);
    printf("inode number: %lld\n",                       (long long)buf.st_ino);
    printf("protection: %o\n",                           (unsigned int)buf.st_mode);
    printf("number of hard links: %lld\n",               (long long)buf.st_nlink);
    printf("user ID of owner: %lld\n",                   (long long)buf.st_uid);
    printf("group ID of owner: %lld\n",                  (long long)buf.st_gid);
    printf("device ID (if special file): %lld\n",        (long long)buf.st_rdev);
    printf("total size, in bytes: %lld\n",               (long long)buf.st_size);
    printf("blocksize for filesystem I/O: %lld\n",       (long long)buf.st_blksize);
    printf("number of 512B blocks allocated %lld\n",     (long long)buf.st_blocks);


    return EXIT_SUCCESS;
}
