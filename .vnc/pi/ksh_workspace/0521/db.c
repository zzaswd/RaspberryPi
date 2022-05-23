#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

static char *host = "localhost";
static char *user = "root";
static char *pass = "kcci";
static char *dbname = "raspi_project";

int main(void){

    MYSQL *conn;
        conn = mysql_init(NULL);

        if(!(mysql_real_connect(conn,host,user,pass,dbname,0,NULL,0))){
	        fprintf(stderr,"ERROR: %s[%d]\n",mysql_error(conn),mysql_errno(conn));
            exit(1);
        }
        printf("Connection Successful!!\n");


		int res = mysql_query(conn,"insert into User_data(NUM,ID,PASS,DATE,TIME) values (null,'asd', '123', curdate(),curtime() )");
        if(!res)
		    printf("inserted %1u rows\n",(unsigned long)mysql_affected_rows(conn));
		else{
			fprintf(stderr,"Insert Error %d : %s\n",mysql_errno(conn),mysql_error(conn));
	    }
		                         

        return EXIT_SUCCESS;
}
