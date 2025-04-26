#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static char *dev = "/dev/oled";
static char msg[84]={0}; //(128/6)*4

int main(void){
	
	int ret = system("/home/pi/snd2txt/snd2txt.sh");
	if (ret == -1){
		perror("system");
		return 1;
	}

	int file_desc;
	
	if ((file_desc = open(dev, O_RDWR)) <0){
		printf("Cannot open device file:%s\n", dev);
		exit(-1);
	}

	/*debug part
	printf("fd = %d\n");
	printf("getpid = %d\n", getpid());
	*/
	FILE *file = fopen("/home/pi/snd2txt/result.txt", "r");
	if (file == NULL){
		perror("fail to open result.txt");
		return 1;
	}

	while(fgets(msg, sizeof(msg), file)){	
	//	printf("please enter a word\n");
	//	fgets(msg, sizeof(msg),file );
		if (write(file_desc, msg, sizeof(msg))<0){
			printf("write failed\n");
		}
		
		sleep(3);
	}
	fclose(file);
	close(file_desc);
	return 0;
}
