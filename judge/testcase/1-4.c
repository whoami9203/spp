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

    printf("%d 2\n", RUNNING->id);
    sleep(1);
    thread_yield();

    printf("%d 3\n", RUNNING->id);
    sleep(1);
    thread_yield();

    printf("%d 4\n", RUNNING->id);
    sleep(1);
    thread_yield();

    printf("%d 5\n", RUNNING->id);
    sleep(1);

    thread_exit();
}
