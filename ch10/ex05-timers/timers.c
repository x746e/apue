#include "apue.h"
#include "common.h"
#include <assert.h>
#include <stdbool.h>
#include <time.h>


void push(time_t timestamp);
bool has_next();
time_t get_next();
time_t pop();
void print_list();


typedef struct node {
    time_t timestamp;
    struct node *next;
} node;

node *head = NULL;


time_t start;


void _reschedule_alarm() {
    time_t now;

    sys_chk(now = time(NULL));


    node *itr = head;

    while (itr) {
        if (itr->timestamp > now) {
            alarm(itr->timestamp - now);
            break;
        }
        itr = itr->next;
    }
}


void _sig_alrm() {
    printf("In _sig_alrm; from start: %lld, now: %lld\n",
            (long long)(time(NULL) - start),
            (long long) time(NULL)
          );
    _reschedule_alarm();
}


void init_timers() {
    struct sigaction act = {{ 0 }};
    sigemptyset(&act.sa_mask);
    act.sa_handler = _sig_alrm;
    sigaction(SIGALRM, &act, NULL);
}


void set_timer(int seconds) {
    time_t now = time(NULL);
    time_t scheduled = now + seconds;
    push(scheduled);
    _reschedule_alarm();
}


int main() {
    init_timers();

    start = time(NULL);
    set_timer(1);
    set_timer(3);
    set_timer(4);
    set_timer(5);
    print_list();
    int volatile k = 0;
    for (int i = 0; i < 1000000; ++i)
        for (int j = 0; j < 2000; ++j)
            k += i * j;
    return EXIT_SUCCESS;
}


// Timestamps to wake up linked list {{{


void push(time_t timestamp) {
    node *new_node = malloc(sizeof(node));
    new_node->timestamp = timestamp;
    new_node->next = NULL;

    if (head == NULL) {
        head = new_node;
        return;
    }

    if (head->timestamp > new_node->timestamp) {
        new_node->next = head;
        head = new_node;
        return;
    }

    node *prev = head;
    node *itr = head->next;
    while (itr) {
        if (itr->timestamp > new_node->timestamp) {
            prev->next = new_node;
            new_node->next = itr;
            return;
        }
        prev = itr;
        itr = itr->next;
    }

    prev->next = new_node;
}


bool has_next() {
    bool ret = head != NULL;
    return ret;
}


time_t get_next() {
    time_t ret = head->timestamp;
    return ret;
}


time_t pop() {
    assert(has_next());
    node old_head = *head;
    free(head);
    head = old_head.next;
    return old_head.timestamp;
}

void print_list() {
    node *itr = head;
    while (itr) {
        printf("%lld -> ", (long long)itr->timestamp);
        itr = itr->next;
    }
    printf("NULL\n");
}
// }}}
