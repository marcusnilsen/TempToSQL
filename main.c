*
 * Build:
 * gcc temptodb.c -o temp -L/usr/local/include/mysql -L/usr/local/lib/mysql/ -lmysqlclient
 *
 * Install to crontab together with:
 * * * * * * /usr/sbin/digitemp_DS9097 -aq -c /etc/digitemp.conf > /tmp/digitemp
 *
 * 13 july 2011 Versjon 1.1:
 * - Made a check to see if the file is empty or not (empty file causing the DB to register 0*C temp if the temp check failed (and made a blank file))
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include </usr/local/include/mysql/mysql.h>

int main (int argc, const char *argv[]) {
        sleep(15); // Wait for digitemp to close the file

        char errortemp[2] = "85";

        /* Read Temp */
        FILE *fp;
        fp = fopen("/tmp/digitemp", "r");
        char tempText[100];
        fgets(tempText, 100, fp);
        fclose(fp);

        if(strlen(tempText) == 0)
        {
                //printf("ingenting i fila\n");
                return(0);
        }

        /* Parse the TEMP text */
//      char tempText[] = "Jun 05 23:10:01 Sensor 0 C: 7.56 F: 45.61";
        char * pch;
        pch = strtok(tempText, " ");
        char * temp;
        int i = 0;
        while (pch != NULL)
        {
                //printf("%s\n",pch);
                pch = strtok(NULL, " ");
                i = i++;
                if(i == 6) {
                        temp = pch;
                }

        if(strncmp(errortemp, temp, 2) == 0)
        {
                /* compare first 2 chars and will return 0 if true */
                //printf("den har reigstrer 85 grader igjen, avslutt\n");
                return(0);
        }

        /*
        char test[200];
        sprintf(test, "INSERT INTO temperatures (sensor_id,temperature) VALUES (\"1\",\"%s\")", temp);
        printf("Temp is: %s\n", temp);
        printf("Query is: %s\n", test);
        */


        /* ------------------- */


        MYSQL *conn;
        MYSQL_RES *res;


        char *server = "10.1.1.10";
        char *user = "sqlUserName";
        char *password = "sqlPassword";
        char *database = "sqlDB";
        char query[200];
        sprintf(query, "INSERT INTO temperatures (sensor_id,temperature) VALUES (\"1\",\"%s\")", temp);
//      char *query = "SELECT * FROM temperatures LIMIT 1";
        int port = 5333;
        int COMPRESS = 1;

        if (COMPRESS == 1) {
                COMPRESS = CLIENT_COMPRESS;
        } else {
                COMPRESS = 0;
        }

        conn = mysql_init(NULL);
        /* Connect to database */
        if (!mysql_real_connect(conn, server, user, password, database, port, NULL, COMPRESS)) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                return(0);
        }

        /* send SQL query */

        /* send SQL query */
        if (mysql_query(conn, query)) {
                fprintf(stderr, "%s\n", mysql_error(conn));
        }

        mysql_close(conn);

        // printf("Husk A Apne porten..\n");

        return(0);
}




