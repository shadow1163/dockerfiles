FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

#RUN docker-php-ext-install mysqli pdo pdo_mysql

RUN apt-get update && apt-get install -y wget apache2 php php-xml php-mbstring php-curl php-memcache php-ldap memcached libapache2-mod-php php-mcrypt php-mysql

COPY 000-default.conf /etc/apache2/sites-available/000-default.conf

COPY simplesamlphp /var/simplesamlphp

CMD apachectl -DFOREGROUND
