#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include "pthread.h"
#include "semaphore.h"


#define LEDOFF 	0
#define LEDON 	1

sem_t number_A,number_B;

void *thread_A(void *arg)
{
    for(int i = 0; i <10;i++)
    {
		sem_wait(&number_A);
        printf("thread_A work\n");
		Led_Status_Switch(LEDON);
        sleep(2);
        sem_post(&number_B);
        printf("thread_A stop\n");
    }
	printf("thread_A exited\n");
	pthread_exit(NULL);
	return NULL;
}

void *thread_B(void *arg)
{
    for(int i = 0; i <10;i++)
    {
		sem_wait(&number_B);
        printf("thread_B work\n");
		Led_Status_Switch(LEDOFF);
        sleep(2);
        sem_post(&number_A);
        printf("thread_B stop\n");
    }
	printf("thread_B exited\n");
	pthread_exit(NULL);
	return NULL;
} 

int Led_Status_Switch(char status)
{
	int fd, retvalue;
	char *filename;
	unsigned char databuf[1];
	
	filename = "/dev/led";

	/* 打开led驱动 */
	fd = open(filename, O_RDWR);
	if(fd < 0){
		printf("file %s open failed!\r\n",filename);
		return -1;
	}

	databuf[0] = status;	/* 要执行的操作：打开或关闭 */

	/* 向/dev/led文件写入数据 */
	retvalue = write(fd, databuf, sizeof(databuf));
	if(retvalue < 0){
		printf("LED Control Failed!\r\n");
		close(fd);
		return -1;
	}
	if(status)
	printf("LED ON\n");
	else
	printf("LED OFF\n");

	retvalue = close(fd); /* 关闭文件 */
	if(retvalue < 0){
		printf("file %s close failed!\r\n",filename);
		return -1;
	}
	return 0;
}
int main()
{
	pthread_t pid,cid;
    sem_init(&number_A,0,1);
    sem_init(&number_B,0,0);

    pthread_create(&pid,NULL,thread_A,NULL);
    pthread_create(&cid,NULL,thread_B,NULL);
    pthread_join(pid,NULL);
    pthread_join(cid,NULL);
    sem_destroy(&number_A);
    sem_destroy(&number_B);
	pthread_exit(NULL);
	return 0;
}
