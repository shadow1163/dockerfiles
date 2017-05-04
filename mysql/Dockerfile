FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN groupadd -r mysql && useradd -r -g mysql mysql

ENV MYSQL_ROOT_PASSWORD welcome

COPY sources.list /etc/apt/sources.list

RUN apt-get update && apt-get install -y --no-install-recommends pwgen openssl wget perl apt-utils gosu

RUN mkdir /docker-entrypoint-initdb.d

RUN {\
        echo mysql-server-5.5 mysql-server/root_password password 'welcome'; \
        echo mysql-server-5.5 mysql-server/root_password_again  password 'welcome'; \
        echo mysql-server-5.5 mysql-server/start_on_boot boolean true; \
    } | debconf-set-selections \
 && apt-get install -y mysql-server && rm -rf /var/lib/mysql && mkdir -p /var/lib/mysql /var/run/mysqld && chown -R mysql:mysql /var/lib/mysql /var/run/mysqld && chmod 777 /var/run/mysqld

RUN sed -Ei 's/^(bind-address|log)/#&/' /etc/mysql/mysql.conf.d/mysqld.cnf \

	&& echo '[mysqld]\nskip-host-cache\nskip-name-resolve' > /etc/mysql/conf.d/docker.cnf

VOLUME /var/lib/mysql

COPY docker-entrypoint.sh /usr/local/bin/

RUN ln -s /usr/local/bin/docker-entrypoint.sh /entrypoint.sh && chmod 777 /entrypoint.sh # backwards compat

ENTRYPOINT ["docker-entrypoint.sh"]
EXPOSE 3306
CMD ["mysqld"]
