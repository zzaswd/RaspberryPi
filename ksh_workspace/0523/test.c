#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>




char capture[] = "sudo raspistill -o /home/pi/Pictures/";
char DATE[10];
char TIME[10];

int main(){

	time_t base = time(NULL);
	struct tm* t = localtime(&base);


	sprintf(DATE,"%d-%d-%d",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
	sprintf(TIME,"%d-%d-%d",t->tm_hour,t->tm_min,t->tm_sec);
	strcat(capture,DATE);
	strcat(capture,",");
	strcat(capture,TIME);
	strcat(capture,".jpg");
	system(capture);
}                                                                                                                                         
