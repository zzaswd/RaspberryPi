#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

char capture[200] = "sudo raspistill -t 1000 -w 400 -h 400 -o /var/www/html/images/";
char capture_date[20];
char capture_time[20];
char in_sql[200]; 

int main(){
	time_t base = time(NULL);
	struct tm* t = localtime(&base);

	MYSQL *conn;
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
    conn = mysql_init(NULL);

    if(!(mysql_real_connect(conn,host,user,pass,dbname,0,NULL,0))){
    	fprintf(stderr,"ERROR: %s[%d]\n",mysql_error(conn),mysql_errno(conn));
        exit(1);
    }
    printf("Connection Successful!!\n");
                                                

    char ser_buff[10] = {0};
    int index = 0,str_len;
    char *pArray[4] = {0};
    char *pToken;
    int res =0;
    printf("Raspberry Startup\n");
    fflush(stdout);
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

			   if(strcmp(ser_buff,"400")==0){
			   		char cap_temp[200] = {0};
			   		strcpy(cap_temp,capture);
					sprintf(capture_date,"%d-%d-%d",t->tm_year+1900,t->tm_mon+1,t->tm_mday);
					sprintf(capture_time,"%d-%d-%d",t->tm_hour,t->tm_min,t->tm_sec);
					strcat(cap_temp,capture_date);
					strcat(cap_temp,",");
					strcat(cap_temp,capture_time);
					strcat(cap_temp,".jpg");
			 		system(cap_temp);
			   }

			   else if(strcmp(ser_buff,"200")==0) {
			   		char id_buff[200] ={0};
			   		int id_idx = 0,id_strlen;
					res = mysql_query(conn,"select ID from User_data");

	    			if(!res){
	    				res_ptr = mysql_store_result(conn);
	    				if(res_ptr){
							printf("Retrived %1u rows\n",(unsigned long)mysql_num_rows(res_ptr));
							sqlrow = mysql_fetch_row(res_ptr);
							strcat(id_buff,sqlrow[0]);
							while((sqlrow = mysql_fetch_row(res_ptr))){
								strcat(id_buff,":");
								strcat(id_buff,sqlrow[0]);
							}
							strcat(id_buff,"LL");
							printf("%s\n",id_buff);
							int i =0 ;
							while(id_buff[i] !='\0'){
								serialPutchar(fd,id_buff[i++]);
								printf("%c",id_buff[i-1]);
								delay(10);
							}
	    				}
	    			}	
	    			else{
	    				fprintf(stderr,"select Error %d : %s\n",mysql_errno(conn),mysql_error(conn));
	    			}
	    			printf("송신 끝\n");
	    			mysql_free_result(res_ptr);

			   }

			   else{
                   pToken = strtok(ser_buff,":");
	               int i = 0;
           
           	    	while(pToken !=NULL){
               			pArray[i] = pToken;
                 		if(++i>4) // 갑자기 데이터가 겹쳐서 들어왔을 때를 방지.
                    		break;
                   		pToken = strtok(NULL,":");
                   	 	printf("seg\n");
   					}


			  		if(atoi(pArray[0])==100){// 회원가입 모드
		    			char id[15];
    	   				char pass[15];
						strcpy(id,pArray[1]);
						strcpy(pass,pArray[2]);					
	    				printf("ID = %s, PASS = %s\n",id,pass);
	    			
					 	sprintf(in_sql,"insert into User_data(NUM,ID,PASS,DATE,TIME) values (null,'%s', '%s', curdate(),curtime() )",id,pass);
					 	printf("%s\n",in_sql);
				 		res = mysql_query(conn,in_sql);	
	    				if(!res)
	    					printf("inserted %1u rows\n",(unsigned long)mysql_affected_rows(conn));
	    				else{
	    					fprintf(stderr,"Insert Error %d : %s\n",mysql_errno(conn),mysql_error(conn));
	    				}
					
				   }
	   			   else if(atoi(pArray[0])==300) {
		    			char id[15];
    	   				char pass[15];
						strcpy(id,pArray[1]);
						strcpy(pass,pArray[2]);					
			   			
						res = mysql_query(conn,"select ID,PASS from User_data");
	
		    			if(!res){
	    					res_ptr = mysql_store_result(conn);
	    					if(res_ptr){
	    						char exist = '0';
								printf("Retrived %1u rows\n",(unsigned long)mysql_num_rows(res_ptr));
								while((sqlrow = mysql_fetch_row(res_ptr))){
									if(strcmp(id,sqlrow[0])==0 && strcmp(pass,sqlrow[1])==0){
										exist = '1';
										break;
									}
								}

								serialPutchar(fd,exist);
								delay(10);
	    					}
	    				}
	    					
	    				else{
	    					fprintf(stderr,"select Error %d : %s\n",mysql_errno(conn),mysql_error(conn));
	    				}
	    				printf("송신 끝\n");
	    				mysql_free_result(res_ptr);

			   	}

			   } 
    		   for(int i = 0 ; i <= str_len; i++){
    				ser_buff[i] = 0;
    				index=0;
    		   }			  

	
			}
			fflush(stdout);
		}
	}
	mysql_close(conn);
	return 0;
}
