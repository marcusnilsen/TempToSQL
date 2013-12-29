/*
 * Build:
 * gcc main.c mysql.c -o temp -L/usr/local/include/mysql -L/usr/local/lib/mysql/ -lmysqlclient
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
#include "mysql.h"

#define TEMPBUFFER 2000


int main (int argc, const char *argv[]) {
    sleep(15); // Wait for digitemp to close the file
    
    /* init db */
    if (!mysqlInit()) {
        printf("Problem connecting to db ...\n");
        return 0;
    }
    
    int i,sensorId = 0;
    char errortemp[2] = "85";
    char tempText[TEMPBUFFER];
    char *temp;

    /* Read Temp */
    FILE *fp;
    fp = fopen("digitemp", "r");

    while (fgets (tempText, TEMPBUFFER, fp) != NULL) {
        /* READ DATA FROM EVERY SENSOR AND PARSE IT */
        printf("Reading data from sensors ...\n");
        
        /* begin with sensor 1 */
        sensorId++;

        /* Split sensor text */
        char *splitString;
        splitString = strtok(tempText, " ");
        
        
        while (splitString != NULL) {
            splitString = strtok(NULL, " ");
            // Find temp value at '6th' word position
            i++;
            if (i == 6) {
                temp = splitString;
            }
        }
        
        if (strncmp(errortemp, temp, 2) == 0) {
            /* return to shell if temp is 85 (sensor bug) */
            fclose(fp);
            mysqlQuit();
            return 0;
        }
        
        printf("Temp is: %s\n", temp);
        
        /* Send to mysql_query function, (SENSORID, TEMP) */
        mysqlInsert(sensorId, temp);
    }
    
    /*
    char test[200];
    sprintf(test, "INSERT INTO temperatures (sensor_id,temperature) VALUES (\"1\",\"%s\")", temp);
    printf("Temp is: %s\n", temp);
    printf("Query is: %s\n", test);
    */
    

    fclose(fp);
    mysqlQuit();
    return 0;
}




