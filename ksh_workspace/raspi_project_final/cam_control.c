#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>


char device[] = "/dev/ttyACM0";
int fd;
unsigned long baud = 9600;

int main(){

	FILE *fp;
    char s_stat[100];
	int count= 0;
	int angle = 90;
	int mode= -1;
	char start[2] = "";
   	if((fd = serialOpen(device,baud))<0){
		fprintf(stderr,"Unable %s\n",strerror(errno));
		exit(1);
 	}
 	if(wiringPiSetup() == -1)    return 1;

	fflush(stdout);
	while(1){
		sleep(1);
	   	if(serialDataAvail(fd)){
			if(serialGetchar(fd)=='C'){
	   			fp = fopen("/var/www/html/cam_control.txt", "r");		
   				fgets(s_stat,100,fp);
   				if(strcmp(s_stat,"LEFT")==0){
					if(angle>0)		angle=angle -10;
					printf("LEFT!\n");
					mode = 1;
		   		}
	
				else if(strcmp(s_stat,"RIGHT")==0){
					if(angle<180)	angle= angle + 10;
					printf("RIGHT!\n");
					mode = 2;
   				}
   				else mode =3;

   				char ang[10] = "";
   				if(mode == 3) sprintf(ang,"C");
   				else sprintf(ang,"%dL",angle);
				int i = 0;
   				while(ang[i] != '\0'){
					serialPutchar(fd,ang[i++]);
					printf("sendchar : %c\n",ang[i-1]);
					delay(10);
   				}
			}
		}
    }
}
