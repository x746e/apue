/**
 * Function to guess file name from file descriptor.  Uses kvm(3) subsystem.
 */
#include <errno.h>

#ifdef __DragonFly__
#define _KERNEL_STRUCTURES
#include <machine/stdint.h>
#include <sys/types.h>
#include <sys/file.h>
#undef _KERNEL_STRUCTURES
#include <kvm.h>
#include <sys/sysctl.h>
#include <sys/filedesc.h>
#include <sys/kinfo.h>
#endif

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#ifdef __FreeBSD__
#include <kvm.h>
#include <sys/sysctl.h>
#define _WANT_FILE
#include <sys/file.h>
#include <sys/filedesc.h>
#include <sys/user.h>
#endif


char kvm_errbuf[_POSIX2_LINE_MAX] = { 0 };
#define kvm_chk_ptr(call) if ((call) == NULL) { \
    fprintf(stderr, "Error in `" #call "` syscall (%s:%d): %s\n", \
            __FILE__, __LINE__, kvm_errbuf); \
    exit(EXIT_FAILURE); \
}

#define kvm_chk(call) errno = 0; \
if ((call) == -1) { \
    fprintf(stderr, "Error in `" #call "` syscall (%s:%d)\n", \
            __FILE__, __LINE__); \
    fprintf(stderr, "kvm_errbuf: %s\n", kvm_errbuf); \
    if (errno != 0) \
        perror("perror"); \
    /*exit(EXIT_FAILURE);*/ \
}


void
guess_file_name(int fd, char *buf, int buflen) {
    kvm_t *kd;
    // This doesn't have to do anything with files, it just initializes kvm.
    kvm_chk_ptr(kd = kvm_openfiles(NULL, NULL, NULL, O_RDONLY, kvm_errbuf));

    // Get kernel structure for current process.
    struct kinfo_proc *kp;
    int cnt;
    kp = kvm_getprocs(kd, KERN_PROC_PID, getpid(), &cnt);
    
    // Get file descriptor table of the process.
    struct filedesc filed;

#ifdef __FreeBSD__
    kvm_chk(kvm_read(kd, (unsigned long)kp->ki_fd, &filed, sizeof(filed))); 
#endif

#ifdef __DragonFly__
    kvm_chk(kvm_read(kd, (unsigned long)kp->kp_fd, &filed, sizeof(filed)));
#endif

    printf("Process has <= %d file descriptors\n", filed.fd_lastfile);


    // Get file structures.
    int nfiles = filed.fd_lastfile + 1;
    struct file **ofiles = malloc(nfiles * sizeof(struct file *));
    assert(ofiles != NULL);
#ifdef __FreeBSD__
    kvm_chk(kvm_read(kd, (unsigned long)filed.fd_ofiles, ofiles,
                     nfiles * sizeof(struct file *)));
#endif
#ifdef __DragonFly__
    kvm_chk(kvm_read(kd, (unsigned long)filed.fd_files, ofiles,
                     nfiles * sizeof(struct file *)));
#endif
    // Loop through the files.
    struct file file;
    ssize_t r;
    for (int i = 0; i <= filed.fd_lastfile; i++) {
        if (ofiles[i] == NULL) continue;
        errno = 0;
        kvm_chk((r = kvm_read(kd, (unsigned long)ofiles[i], &file, sizeof(struct file))));
        printf("Filed #%d: %zd bytes read\n", i, r);
    }

    kvm_chk(kvm_close(kd));
}


/* int */
/* main() { */
/*     char buf[PATH_MAX]; */
/*     guess_file_name(0, buf, sizeof buf); */
/*     printf("guessed file name: %s\n", buf); */
/*     return EXIT_SUCCESS; */
/* } */
