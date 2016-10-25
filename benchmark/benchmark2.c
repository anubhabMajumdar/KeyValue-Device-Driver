#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <keyvalue.h>
#include <fcntl.h>

int keyvalue[10];

void *putChar()
{
	char data[4096];
	int devfd;
	int key;
	
	key = rand() % 10;
	
	memset(data, 0, 4096);
    a = rand();				
    sprintf(data,"%d",a);
    data = rand();
	
	tid = kv_set(devfd,i%number_of_keys,strlen(data),data);
        
	
