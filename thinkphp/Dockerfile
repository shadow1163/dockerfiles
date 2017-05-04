FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

COPY sources.list /etc/apt/sources.list

RUN apt-get update && apt-get install -y php7.0 apache2 git libapache2-mod-php

RUN git clone https://github.com/top-think/think /opt/tp5

RUN git clone https://github.com/top-think/framework /opt/tp5/thinkphp 

COPY 000-default.conf /etc/apache2/sites-available/

CMD service apache2 restart


