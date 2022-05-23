#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <mysql/mysql.h>

static char *host = "localhost";
static char *user = "root";
static char *pass = "kcci";
static char *dbname = "raspi_project";

char device[] = "/dev/ttyACM0";
int fd;
unsigned long baud = 9600;
char capture[200] = "sudo service motion ";
int main(){
	int compare = 0;
	MYSQL *conn;
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
    conn = mysql_init(NULL);

    if(!(mysql_real_connect(conn,host,user,pass,dbname,0,NULL,0))){
    	fprintf(stderr,"ERROR: %s[%d]\n",mysql_error(conn),mysql_errno(conn));
        exit(1);
    }
    printf("Connection Successful!!\n");
                                                

	while(1){
		char temp[100];
		sprintf(temp,"select status from ON_OFF");
		int res = mysql_query(conn,temp);
		if(!res){
    	    res_ptr = mysql_store_result(conn);
    		if(res_ptr){
        		printf("Retrived %1u rows\n",(unsigned long)mysql_num_rows(res_ptr));
        	  	sqlrow = mysql_fetch_row(res_ptr);
            	printf("%s\n",sqlrow[0]);
            	if(strcmp(sqlrow[0],"on") == 0 && compare == 0){
					char temp[40] = {0};
					sprintf(temp,"%s",capture);
            		strcat(temp,"start");
					compare = 1;
            		system(temp);
            	}
            	else if(strcmp(sqlrow[0],"off")== 0 && compare == 1){
					char temp[40] = {0};
					sprintf(temp,"%s",capture);
            		strcat(temp,"stop");
					compare = 0;
            		system(temp);
            	}
        	}
 		}
 	
 		else{
			fprintf(stderr,"select Error %d: %s\n",mysql_errno(conn),mysql_error(conn));
 		}
 		

	}
	mysql_free_result(res_ptr);
   	mysql_close(conn);

    return EXIT_SUCCESS;

}
