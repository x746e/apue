#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/queue.h>
#include <sys/user.h>

#include <netinet/in.h>
#include <libprocstat.h>


void
guess_file_name(int fd, char *buf, int buflen) {
    struct procstat *prstat;
    struct kinfo_proc *p;
    struct filestat *fst;
    struct filestat_list *head;
    unsigned int cnt;
    prstat = procstat_open_sysctl();

    p = procstat_getprocs(prstat, KERN_PROC_PID, getpid(), &cnt);
    assert(cnt == 1);

    head = procstat_getfiles(prstat, p, 0 /* `mmapped`, have no idea what is that */);
    assert(head != NULL);

    STAILQ_FOREACH(fst, head, next) {
        if (fst->fs_fd == fd) {
            char *path = fst->fs_path != NULL ? fst->fs_path : "-";
            if (strlcpy(buf, path, buflen) >= buflen) {
                // TODO: Use err_quit.
                fprintf(stderr, "Buffer is too small in guess_file_name\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }

    procstat_freefiles(prstat, head);
    procstat_freeprocs(prstat, p);
    procstat_close(prstat);
}


/* int */
/* main() { */
/*     char buf[PATH_MAX]; */
/*     guess_file_name(0, buf, sizeof buf); */
/*     printf("guessed file name: %s\n", buf); */
/*     return EXIT_SUCCESS; */
/* } */
