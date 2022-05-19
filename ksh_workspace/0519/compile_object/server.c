#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringSerial.h>
char device[] = "/dev/ttyACM0";
int fd;
unsigned long baud = 9600;


int main(){
	char ser_buff[10] = {0};
	int index = 0, state1, state2,state3,str_len;
	char *pArray[3] = {0};
	char *pToken;
	printf("Raspberry Startup\n");
	fflush(stdout);
	fflush(stdin);
	if((fd = serialOpen(device, baud)) < 0 ){
		fprintf(stderr, "Unable to open %s\n ", strerror(errno));
		exit(1);
	}

	if(wiringPiSetup() == -1)
		return 1;

	while(1){
		if(serialDataAvail(fd)){
			ser_buff[index++] = serialGetchar(fd);
			if(ser_buff[index-1] == 'L')
			{
				ser_buff[index-1] = '\0';
				str_len = strlen(ser_buff);
				printf("ser_buff = %s\n",ser_buff);
				pToken = strtok(ser_buff,":");
				int i = 0;
				while(pToken !=NULL){
					pArray[i] = pToken;
					if(++i>3)
						break;
					pToken = strtok(NULL,":");

				}

				state1 = atoi(pArray[0]);
				state2 = atoi(pArray[1]);
				state3 = atoi(pArray[2]);
				printf("state1 = %d, state2 = %d, state3 = %d\n",state1,state2,state3);
				for(int i = 0; i <= str_len; i++){
					ser_buff[i] = 0;
				index = 0;

				}

			}

			fflush(stdout);
		}
	}
	return 0;
}

