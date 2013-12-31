Read temperatures from a digitemp output file and insert them to a database
=============

1. Edit mysql.c and change your database user credentials.

2. Compile with mysql header files. Use gcc or cc.

	gcc main.c mysql.c -o temptosql -L/usr/local/include/mysql -L/usr/local/lib/mysql/ -lmysqlclient

3. Run digitemp in crontab as often as you want the data (ex: every 5min):

	*/5 * * * * /usr/sbin/digitemp_DS9097 -aq -c /etc/digitemp.conf > /tmp/digitemp

4. Run the file you compiled in crontab, execute at the same time as above:

	*/5 * * * * /path/to/compiled/program/temptosql


*TempToSQL 2011-2014 @marcusnilsen (.com)*
