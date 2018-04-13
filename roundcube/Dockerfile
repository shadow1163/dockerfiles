FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

#COPY sources.list /etc/apt/sources.list

RUN apt-get update && apt-get install -y wget php-xml php-mbstring php-intl php-zip php-pear zip unzip git composer lamp-server^

#copy php.ini to instance
COPY php.ini /etc/php/7.0/apache2/php.ini

RUN wget https://github.com/roundcube/roundcubemail/releases/download/1.3.5/roundcubemail-1.3.5-complete.tar.gz && tar zxf roundcubemail-1.3.5-complete.tar.gz && mv roundcubemail-1.3.5 /var/www/roundcube && chown -R www-data:www-data /var/www/roundcube/ && chmod 775 /var/www/roundcube/temp/ /var/www/roundcube/logs/

COPY roundcube.conf /etc/apache2/sites-available

RUN a2dissite 000-default && a2ensite roundcube && a2enmod rewrite && apache2ctl restart

