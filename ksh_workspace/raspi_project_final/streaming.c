#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void delay(clock_t n){
	clock_t start = clock();
	while(clock()-start<n);
}
int main(){

	FILE *fp;
    char s_stat[100];
    int cam_status = 0;
	system("sudo service motion start");
	int count= 0;
    while(1){
    	count++;
    	if(count == 100000){
    		fp = fopen("/var/www/html/cam_status.txt", "r");
    		fgets(s_stat,100,fp);
    		if(strcmp(s_stat,"ON")==0 && cam_status == 0){
				cam_status = 1;
				system("sudo motion");
				printf("ON!\n");
    		}
			else if(strcmp(s_stat,"OFF")==0 && cam_status == 1){
				cam_status = 0;
				system("sudo service motion stop");
				printf("OFF!\n");
			
    		}
  			count = 0;
    	}
    }
}
