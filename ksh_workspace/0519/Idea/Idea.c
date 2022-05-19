#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <mysql/mysql.h>
static char *host = "localhost";
static char *user = "root";
static char *pass = "kcci";
static char *dbname = "test";

char device[] = "/dev/ttyACM0";
int fd;
unsigned long baud = 9600;


int main(){

	MYSQL *conn;
	conn = mysql_init(NULL);
	char in_sql[200] = {0};
	int bus_num=0;
	int sql_index, flag = 0;
	int res = 0 ;
	
	if(!(mysql_real_connect(conn,host,user,pass,dbname,0,NULL,0))){
		fprintf(stderr,"ERROR : %s[%d]\n",mysql_error(conn),mysql_errno(conn));
		exit(1);
	}
	else printf("Connection Successful!\n");


	
	char ser_buff[10] = {0};

	char now_bus[15];
	char to_bus[15];
	
	int index = 0,str_len;
	char *pArray[3] = {0};
	char *pToken;

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
				flag =1;
				ser_buff[index-1] = '\0';
				str_len = strlen(ser_buff);
				printf("ser_buff = %s\n",ser_buff);
				pToken = strtok(ser_buff,":");
				int i = 0;
				while(pToken !=NULL){
					pArray[i] = pToken;
					if(++i>5)
						break;
					pToken = strtok(NULL,":");

				}
				strcpy(now_bus,pArray[0]);
				strcpy(to_bus,pArray[1]);
				bus_num = atoi(pArray[2]);
				printf("NOW_BUS_STOP = %s, TO_BUS_STOP = %s, BUS_NUMBER = %d\n",pArray[0],pArray[1],bus_num);

				for(int i = 0; i <= str_len; i++){
					ser_buff[i] = 0;
					index = 0;
				}
			//	if(temp<100 && humi<100){ 			  // 튀는 값 제거
					if(flag == 1){
						sprintf(in_sql,"insert into Idea(NOW_BUS_STOP, TO_BUS_STOP, BUS_NUMBER, time) values ('%s', '%s', %d, curtime() )",now_bus,to_bus,bus_num);
						printf("%s\n",in_sql);
						res = mysql_query(conn,in_sql);


						printf("res : %d\n",res);
						if(!res)
							printf("inserted %1u rows\n",(unsigned long)mysql_affected_rows(conn));
						else
							fprintf(stderr,"Insert Error %d : %s\n",mysql_errno(conn),mysql_error(conn));
					}
				
			}
			fflush(stdout);
		}
	}
	mysql_close(conn);
	return 0;
}

