#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdint.h>
#include "hello_bitmap.h"
#include "oasis_bitmap.h"
#include "ssd1306_mmap.h"

static char *dev = "/dev/oled";
static char msg[84]={0}; //(128/6)*4

int main(void){
	int file_desc;
	
	int ret = system("/home/pi/snd2txt/snd2txt.sh");
	if (ret == -1){
		perror("system");
		return 1;
	}	
	
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
	

	int contrast = 0xff;
	int i = 0;
	while(fgets(msg, sizeof(msg), file)){	
		if (write(file_desc, msg, sizeof(msg))<0){
			printf("write failed\n");
		}
		
		//adjust contrast
		contrast >>= i++; 
		ioctl(file_desc, OLED_CONTRAST,&contrast );
		
		sleep(4);
	}
	ioctl(file_desc, OLED_CLEAR);
	
	//draw bitmap using mmap()
	uint8_t *fb = mmap(NULL, 513, PROT_READ|PROT_WRITE, MAP_SHARED,file_desc,0);
	if (fb == MAP_FAILED){
		perror("mmap failed");
		exit(1);
	}
	ioctl(file_desc, OLED_CLEAR_FB);
	memcpy(fb+1, oasis_bitmap, 512);

	if (ioctl(file_desc, OLED_MMAP_DRAW)<0){
		perror("ioctl failed");
	}
	sleep(3);
	ioctl(file_desc, OLED_CLEAR_FB);
	ioctl(file_desc, OLED_MMAP_DRAW);
	munmap(fb, 513);
	fclose(file);
	close(file_desc);
	return 0;
}
