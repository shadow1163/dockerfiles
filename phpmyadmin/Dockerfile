FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

COPY sources.list /etc/apt/sources.list

#RUN apt-get update && apt-get install -y phpmyadmin
RUN apt-get update

RUN {\
	echo phpmyadmin phpmyadmin/root_password password 'password';\
	echo phpmyadmin phpmyadmin/root_password_again  password 'password'; \
} | debconf-set-selections \
 && apt-get install phpmyadmin
