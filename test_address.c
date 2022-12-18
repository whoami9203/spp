#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<math.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/mman.h>

#define ARRAY_SIZE 0x40000
#define MALLOC_SIZE 0x100000

char array[ARRAY_SIZE];

void err_sys(const char* x){
	perror(x);
	exit(1);
}

int main(int argc, char const *argv[])
{
	int id;
	char *ptr;
	void *area;

	printf("array[] from %p to %p\n", (void*)&array[0], (void*)&array[ARRAY_SIZE]);

	printf("stack around %p: id\n", (void*)&id);

	printf("stack around %p: ptr\n", (void*)&ptr);

	if((ptr = malloc(MALLOC_SIZE)) == NULL)
		err_sys("malloc error");
	printf("malloc from %p to %p\n", (void*)ptr, (void*)ptr+MALLOC_SIZE);

	if((ptr = mmap(0, ARRAY_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED)
		err_sys("mmap error");
	printf("mmap from %p to %p\n", (void*)ptr, (void*)ptr+ARRAY_SIZE);

	sleep(20);

	if((ptr = mmap(0, ARRAY_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0)) == MAP_FAILED)
		err_sys("mmap error");
	printf("mmap from %p to %p\n", (void*)ptr, (void*)ptr+ARRAY_SIZE);

	while(1);

	exit(0);
}
