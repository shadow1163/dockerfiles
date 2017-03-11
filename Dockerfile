FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

#ENV DEBIAN_FRONTEND noninteractive

#RUN echo "export LC_ALL=C"

COPY sources.list /etc/apt/sources.list

RUN apt-get update

RUN debconf-set-selections <<< 'mysql-server mysql-server/root_password password welcome'
RUN debconf-set-selections <<< 'mysql-server mysql-server/root_password_again password welcome'

RUN apt-get install -y mysql-server
