# Pull base image
FROM ubuntu:16.04

MAINTAINER shadow1163 "674602286@qq.com"

ENV MYSQL_USER=mysql \
	MYSQL_DATA_DIR=/var/lib/mysql \
	MYSQL_DATA_DIR=/run/mysqld \
	MYSQL_LOG_DIR=/var/log/mysql

RUN echo "deb http://cn.archive.ubuntu.com/ubuntu/ xenial main restricted" > /etc/apt/sources.list

RUN apt-get update
RUN apt-get install apparmor
RUN apt-get install mysql-common mysql-server-5.7

COPY entrypoint.sh /sbin/entrypoint.sh
RUN chmod 755 /sbin/entrypoint.sh

EXPOSE 3306/tcp
VOLUME ["${MYSQL_DATA_DIR}","${MYSQL_RUN_DIR}"]
ENTERPOINT ["/sbin/entrypoint.sh"]
CMD ["/usr/bin/mysqld_safe"]
