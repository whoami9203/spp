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
    printf("%d\n", RUNNING->id);
    sleep(1);
    thread_exit();
}
