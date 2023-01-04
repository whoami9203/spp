#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "threadtools.h"

int timeslice = 3;
sigset_t base_mask, tstp_mask, alrm_mask;
struct tcb *ready_queue[], *waiting_queue[];
int rq_size, rq_current, wq_size;
jmp_buf sched_buf;

void f1(int id, int arg);
void f2(int id, int arg);
void f3(int id, int arg);

/*
 * This function turns stdin, stdout, and stderr into unbuffered I/O, so:
 *   - you see everything your program prints in case it crashes
 *   - the program behaves the same if its stdout doesn't connect to a terminal
 */
void unbuffered_io() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

/*
 * Initializes the signal masks and the signal handler.
 */
void init_signal() {
    /* initialize the signal masks */
    sigemptyset(&base_mask);
    sigaddset(&base_mask, SIGTSTP);
    sigaddset(&base_mask, SIGALRM);
    sigemptyset(&tstp_mask);
    sigaddset(&tstp_mask, SIGTSTP);
    sigemptyset(&alrm_mask);
    sigaddset(&alrm_mask, SIGALRM);

    /* initialize the signal handlers */
    signal(SIGTSTP, sighandler);
    signal(SIGALRM, sighandler);

    /* block both SIGTSTP and SIGALRM */
    sigprocmask(SIG_SETMASK, &base_mask, NULL);
}

/*
 * Threads are created nowhere else but here.
 */
void init_threads() {
    thread_create(f1, 1412819413, 0);
    thread_create(f2, 1992872216, 0);
    thread_create(f3, 913492521, 0);
}

/* 
 * Calls the scheduler to begin threading.
 */
void start_threading() {
    alarm(timeslice);
    scheduler();
}

int main() {
    unbuffered_io();
    init_signal();
    init_threads();
    start_threading();
}
