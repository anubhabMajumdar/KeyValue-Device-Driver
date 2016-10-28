#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <time.h>
#include <keyvalue.h>
#include <fcntl.h>

const int sleeptime = 1;
const int cf = 3;

void *setcall(int devfd)
{
	int a;
    int tid;
    int k;
    __u64 size;
    char data[1024];
    
    while (1){
    memset(data, 0, 1024);
    a = rand();
    sprintf(data,"%d",a);
    
    k = rand()%cf;
    
    tid = kv_set(devfd,k,strlen(data),data);
    printf("SET --> S\tTID=%d\tKEY=%d\tDATA SIZE=%d\tDATA=%s\n",tid,k,strlen(data),data);	
    sleep(sleeptime);
    }
}	

void *getcall(int devfd)
{
	int tid;
    int k;
    __u64 size;
    char data[1024];
    
    while (1){
    memset(data, 0, 1024);
    
    k = rand()%cf;
    
    tid = kv_get(devfd,k,&size,&data);
    printf("GET --> S\tTID=%d\tKEY=%d\tDATA SIZE=%d\tDATA=%s\n",tid,k,strlen(data),data);
    sleep(sleeptime);
    }
}	

void *deletecall(int devid)
{
	int tid;
	int k;
	
	while (1){
	
	k = rand()%cf;
	
	tid = kv_delete(devid, k);
	
	printf("DELETE --> S\tTID=%d\tKEY=%d\n", tid, k);
	sleep(sleeptime);
	}
}

int main(int argc, char *argv[])
{
    int devfd, i;
    pthread_t threads[15];
    
    devfd = open("/dev/keyvalue",O_RDWR);
    if(devfd < 0)
    {
        fprintf(stderr, "Device open failed");
        exit(1);
    }
	
	/*
	pthread_create(&threads[0], NULL, setcall, devfd);
	pthread_create(&threads[1], NULL, setcall, devfd);
	
	pthread_create(&threads[2], NULL, getcall, devfd);
	pthread_create(&threads[3], NULL, getcall, devfd);
	
	pthread_create(&threads[4], NULL, deletecall, devfd);
	pthread_create(&threads[5], NULL, deletecall, devfd);
	*/
	
	for(i=0;i<5;i++)
		pthread_create(&threads[i], NULL, setcall, devfd);
	
	for(i=5;i<10;i++)
		pthread_create(&threads[i], NULL, getcall, devfd);
	
	for(i=10;i<15;i++)
		pthread_create(&threads[i], NULL, deletecall, devfd);
		
	
	for (i=0;i<15;i++)
		pthread_join(threads[i], NULL);
	
	return 0;
	 	   
}    
