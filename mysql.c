#include </usr/local/include/mysql/mysql.h>

#include <string.h>
#include <stdio.h>

static MYSQL *conn;
static MYSQL_RES *res;

int mysqlInit () {
    /* init db connection */
    conn = mysql_init(NULL);
    
    char *server = "10.1.1.10";
    char *user = "sqlUserName";
    char *password = "sqlPassword";
    char *database = "sqlDB";
    
    int port = 5333;
    int COMPRESS = 1;
    
    if (COMPRESS == 1) {
        COMPRESS = CLIENT_COMPRESS;
    } else {
        COMPRESS = 0;
    }
    
    /* Connect to database */
    if (!mysql_real_connect(conn, server, user, password, database, port, NULL, COMPRESS)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return 0;
    }
    return 0;
}

int mysqlInsert (int sensor, char *temp) {
    /* create query string */
    char query[500];
    sprintf(query, "INSERT INTO temperatures (sensor_id,temperature) VALUES (\"%d\",\"%s\")", sensor, temp);
    
    /* send SQL query */
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }
    return 0;
}

void mysqlQuit() {
    /* close db connection */
    mysql_close(conn);
}
