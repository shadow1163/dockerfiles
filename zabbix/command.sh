# create mysql
docker run --name mysql-server -t -d -e MYSQL_DATABASE="zabbix" -e MYSQL_USER="zabbix" -e MYSQL_PASSWORD="password" -e MYSQL_ROOT_PASSWORD="password" mysql:5.7 --character-set-server=utf8 --collation-server=utf8_bina
# create gateway
docker run --name zabbix-java-gateway -t -d zabbix/zabbix-java-gateway:latest
# create zabbix
docker run --name zabbix-server-mysql -t -e DB_SERVER_HOST="mysql-server" -e MYSQL_DATABASE="zabbix" -e MYSQL_USER="zabbix" -e MYSQL_PASSWORD="password" -e MYSQL_ROOT_PASSWORD="password" -e ZBX_JAVAGATEWAY="zabbix-java-gateway" --link mysql-server:mysql --link zabbix-java-gateway:zabbix-java-gateway -p 10051:10051 -d zabbix/zabbix-server-mysql:latest
# create web interface
docker run --name zabbix-web-apache-mysql -t -e DB_SERVER_HOST="mysql-server" -e MYSQL_DATABASE="zabbix" -e MYSQL_USER="zabbix" -e MYSQL_PASSWORD="password" -e MYSQL_ROOT_PASSWORD="password" --link mysql-server:mysql --link zabbix-server-mysql:zabbix-server -p 80:80 -d zabbix/zabbix-web-apache-mysql
