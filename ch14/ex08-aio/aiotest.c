#include "apue.h"
#include "common.h"
#include <aio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>

#define NBUF 3
#define BSZ 8


int main(int argc, char *argv[]) {
    int fd;
    sys_chk(fd = open(argv[1], O_RDONLY));

    bool done = false;
    int err;
    int nread;
    off_t offset = 0;
    char *buffers[NBUF];
    struct aiocb *aiolist[NBUF];

    // Allocate memory.
    for (int i = 0; i < NBUF; ++i) {
        buffers[i] = malloc(BSZ);
        aiolist[i] = calloc(1, sizeof(struct aiocb));
    }

    // Init aio structures the first time.
    for (int i = 0; i < NBUF; ++i) {
        aiolist[i]->aio_fildes = fd;
        aiolist[i]->aio_offset = offset;
        aiolist[i]->aio_nbytes = BSZ;
        aiolist[i]->aio_buf = buffers[i];
        aiolist[i]->aio_sigevent.sigev_notify = SIGEV_NONE;
        offset += BSZ;
        sys_chk(aio_read(aiolist[i]));
    }

    // Wait while first batch completes.
    sys_chk(aio_suspend(aiolist, NBUF, NULL));

    // Loop over aio structures; for completed ones print whatever they read;
    // enqueue more read requests; when EOF is reached, break out of the loop.
    while (!done) {
        for (int i = 0; i < NBUF; ++i) {
            err = aio_error(aiolist[i]);
            switch (err) {
                case EINPROGRESS:
                    // Wait.
                    break;
                case 0:
                    // Print the buffer;
                    nread = aio_return(aiolist[i]);
                    if (nread < 0) {
                        err_exit(nread, "AIO read error");
                    }
                    printf("AIO request %d: read %d bytes: ", i, nread);
                    for (int j = 0; j < nread; ++j) {
                        printf("%c", ((char*)(aiolist[i]->aio_buf))[j]);
                    }
                    printf("\n");
                    if (nread < aiolist[i]->aio_nbytes) {
                        printf("That's less than was requested\n");
                    }
                    if (nread == 0) {
                        done = true;
                    }
                    // Falling through.
                case ECANCELED:
                    // Reuse the struct.
                    aiolist[i]->aio_offset = offset;
                    offset += BSZ;
                    sys_chk(aio_read(aiolist[i]));
                    break;
                default:
                    err_exit(err, "AIO request %d failed", i);
            }
        }
        sys_chk(aio_suspend(aiolist, NBUF, NULL));
    }
    

    return EXIT_SUCCESS;
}
