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

    async_read(5);

    if (strncmp("mass\n", RUNNING->buf, 5))
        printf("WA");

    printf("%d 1\n", RUNNING->id);
    sleep(1);
    async_read(7);

    if (strncmp("charge\n", RUNNING->buf, 7))
        printf("WA");
    printf("%d 2\n", RUNNING->id);
    sleep(1);
    thread_exit();
}

void f2(int id, int arg) {
    thread_setup(id, arg);

    printf("%d 1\n", RUNNING->id);
    sleep(1);
    thread_yield();

    printf("%d 2\n", RUNNING->id);
    sleep(1);
    thread_exit();
}
