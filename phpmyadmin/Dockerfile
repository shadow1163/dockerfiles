FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN groupadd -r mysql && useradd -r -g mysql mysql
#
ENV MYSQL_ROOT_PASSWORD welcome
#
COPY sources.list /etc/apt/sources.list
#
#RUN apt-get update
RUN apt-get update && apt-get install -y --no-install-recommends pwgen openssl wget perl apt-utils gosu

RUN {\
        echo mysql-server-5.5 mysql-server/root_password password 'password'; \
        echo mysql-server-5.5 mysql-server/root_password_again  password 'password'; \
        echo mysql-server-5.5 mysql-server/start_on_boot boolean true; \
    } | debconf-set-selections \
 && apt-get install -y mysql-server && rm -rf /var/lib/mysql && mkdir -p /var/lib/mysql /var/run/mysqld && chown -R mysql:mysql /var/lib/mysql /var/run/mysqld && chmod 777 /var/run/mysqld

RUN sed -Ei 's/^(bind-address|log)/#&/' /etc/mysql/mysql.conf.d/mysqld.cnf \

	&& echo '[mysqld]\nskip-host-cache\nskip-name-resolve' > /etc/mysql/conf.d/docker.cnf

RUN {\
        echo phpmyadmin	phpmyadmin/remote/host	select	localhost; \
        echo phpmyadmin	phpmyadmin/dbconfig-install	boolean	true; \
        echo phpmyadmin	phpmyadmin/mysql/app-pass	password 'welcome'; \
        echo phpmyadmin	phpmyadmin/app-password-confirm	password 'welcome'; \
        echo phpmyadmin	phpmyadmin/password-confirm	password 'welcome'; \
        echo phpmyadmin	phpmyadmin/mysql/admin-pass	password 'welcome'; \
        echo phpmyadmin	phpmyadmin/database-type	select	mysql; \
        echo phpmyadmin	phpmyadmin/reconfigure-webserver	multiselect	apache2; \
    } | debconf-set-selections \
&& apt-get install -y phpmyadmin
