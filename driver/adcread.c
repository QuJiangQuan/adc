#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main (int argc,char *argv[]){
	int val = 0;
	int fd = open ("/dev/myAdc",O_RDWR);
	if (fd < 0){
		perror("open");
		return -1;
	}
	while (1){
		read(fd,&val,4);
		printf("val=%d\r\n",val);
		sleep(1);
	}
}