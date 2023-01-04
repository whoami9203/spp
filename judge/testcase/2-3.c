#include "threadtools.h"

#include <setjmp.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void f1(int id, int arg) {
    thread_setup(id, arg);
    
    printf("%d 1\n", RUNNING->id);
    sleep(1);
    thread_yield();

    // TSTP, switch to 2

    printf("%d 2\n", RUNNING->id);
    sleep(1);
    thread_yield();

    // ALRM, switch to 2

    printf("%d 3\n", RUNNING->id);
    sleep(1);
    thread_yield();

    // TSTP, switch to 2

    printf("%d 4\n", RUNNING->id);
    sleep(1);
    thread_yield();

    // TSTP, switch to 2

    printf("%d 5\n", RUNNING->id);
    sleep(1);
    thread_yield();

    // TSTP, switch to 3

    printf("%d 6\n", RUNNING->id);
    sleep(1);
    thread_exit();

    // done

}

void f2(int id, int arg) {
    thread_setup(id, arg);

    printf("%d 1\n", RUNNING->id);
    sleep(1);
    thread_yield();

    // TSTP, switch to 3

    printf("%d 2\n", RUNNING->id);
    sleep(1);
    thread_yield();

    // TSTP, switch to 1

    printf("%d 3\n", RUNNING->id);
    sleep(1);
    thread_yield();

    // ALRM, 3 ready, switch to 3

    printf("%d 4\n", RUNNING->id);
    sleep(1);
    async_read(6);

    // 3 ready, 2 leave, switch to 3

    if (strncmp("hh dd\n", RUNNING->buf, 6))
        printf("2-5 WA\n");
    printf("%d 5\n", RUNNING->id);
    sleep(1);
    thread_exit();

    // switch to 1
}

void f3(int id, int arg) {
    thread_setup(id, arg);

    async_read(9);

    // immediately leave, switch to 1
    if (strncmp("aoeugcrl\n", RUNNING->buf, 9))
        printf("3-1 WA\n");
    printf("%d 1\n", RUNNING->id);
    sleep(1);
    async_read(5);

    // leave, switch to 1

    if (strncmp("htns\n", RUNNING->buf, 5))
        printf("3-2 WA\n");
    printf("%d 2\n", RUNNING->id);
    sleep(1);
    thread_yield();

    // ALRM, switch to 1

    printf("%d 3\n", RUNNING->id);
    sleep(1);
    thread_exit();

    // 2 ready, switch to 2
}
