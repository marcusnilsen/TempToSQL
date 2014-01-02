/*
 * Build:
 * gcc main.c mysql.c -o temp -L/usr/local/include/mysql -L/usr/local/lib/mysql/ -lmysqlclient
 *
 * Install to crontab together with:
 * * * * * * /usr/sbin/digitemp_DS9097 -aq -c /etc/digitemp.conf > /tmp/digitemp
 *
 * Version history before GIT:
 * 
 * 24 July 2011 Version v1.3: bugfix
 * 13 july 2011 Version v1.2:
 * - Made a check to see if the file is empty or not (empty file causing the DB to register 0*C temp if the temp check failed (and made a blank file))
 * 9 July 2011 Version v1.1: bugfix, added quick fix for a 85*C bug in the sensor (??)
 * 6 June 2011 Version v1.0: First version finished. Started to use it live.
 *
 * tempToSQL - @marcusnilsen (.com)
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "mysql.h"

#define TEMPBUFFER 2000
#define PATHTOTEMPFILE digitemp

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
    fp = fopen(PATHTOTEMPFILE, "r");

    while (fgets (tempText, TEMPBUFFER, fp) != NULL) {

        /* READ DATA FROM EVERY SENSOR AND PARSE IT */
        printf("Reading data from sensors ...\n");
        
	/* Sensor ID to be saved to db, start with sensor 1 */
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

    fclose(fp);
    mysqlQuit();
    return 0;
}




