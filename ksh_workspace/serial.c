#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringSerial.h>

char device[] = "/dev/ttyACM0"; //  포트 경로
int fd;
unsigned long baud = 9600; // baud rate 일치
int main(){

	printf("%s\n","Raspberry Startup");
	fflush(stdout); // 한번 지워주고
	if((fd = serialOpen(device,baud))<0)   // 디바이스 정보와 통신 속도 예외처리
	{
		fprintf(stderr,"Unable Serial device %s\n",strerror(errno));
		exit(1);
	}

	if(wiringPiSetup()==-1) return -1; // 예외 처리

	while(1){
		if(serialDataAvail(fd)){
			char newChar = serialGetchar(fd);  //fd가 핸들러임. 
			printf("%c" , newChar);
			fflush(stdout);
		}
	}
}

