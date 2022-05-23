#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(){

	FILE *fp;
    char s_stat[100];
    char s[200];
    int cam_status = 0;
	int count= 0;

	system("sudo service motion start");	
	while(1){
		sleep(3);
		
    		fp = fopen("/var/www/html/cam_status.txt", "r");
		
    		fgets(s_stat,100,fp);
    		sprintf(s,s_stat);
    		if(strcmp(s_stat,"ON")==0 && cam_status == 0){
				cam_status = 1;
				system("sudo motion");
				printf("ON!\n");
    		}
 
			else if(strcmp(s_stat,"OFF")==0 && cam_status == 1){
				cam_status = 0;
				printf("OFF!\n");
				system("sudo service motion restart");
			
    		}
    }
}
