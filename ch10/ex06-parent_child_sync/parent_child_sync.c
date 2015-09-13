#include "apue.h"
#include "common.h"
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>



void init_counter();
void inc_counter(char*);


bool exiting = false;


/* void sig_int  */

void sig_chld() {
    int status;
    pid_t pid = wait(&status);

    printf("Got SIGCHLD for %d\n", pid);
    printf("Signalled? %d\n", WIFSIGNALED(status));
    if (WIFSIGNALED(status)) {
        printf("Signal: %s\n", strsignal(WTERMSIG(status)));
    }
}


int main() {
    pid_t pid;

    signal(SIGCHLD, sig_chld);

    init_counter();

    pid = fork();
    if (pid < 0) {
        err_sys("fork error");
    } else if (pid == 0) {  // child
        while (true) {
            TELL_WAIT();
            WAIT_PARENT();
            inc_counter("child");
            TELL_PARENT(getppid());
        }
    } else {  // parent
        TELL_WAIT();
        while (true) {
            inc_counter("parent");
            sleep(1);
            TELL_CHILD(pid);
            TELL_WAIT();
            WAIT_CHILD();
        }
    }

    return EXIT_SUCCESS;
}


void init_counter() {
    FILE *f = fopen("counter.txt", "w+");
    fprintf(f, "%d", 0);
    fclose(f);
}


void inc_counter(char *who) {
    int counter;
    FILE *f;
   
    f = fopen("counter.txt", "r");
    fscanf(f, "%d", &counter);
    fclose(f);

    counter++;

    f = fopen("counter.txt", "w");
    printf("%s: increasing counter to %d\n", who, counter);
    fprintf(f, "%d", counter);
    fclose(f);
}
