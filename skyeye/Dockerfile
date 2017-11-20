FROM ubuntu:14.04

MAINTAINER shadow1163 (674602286@qq.com)

COPY sources.list /etc/apt/sources.list
#
#RUN apt-get update
RUN apt-get update && apt-get install -y libgtk2.0-dev pkg-config libatk1.0-dev  libpango1.0-dev libfreetype6-dev libglib2.0-dev libx11-dev libncurses5-dev libxpm-dev libltdl-dev zlib1g-dev libbz2-dev libssl-dev libncurses5-dev  libsqlite3-dev libreadline-dev tk-dev libgdbm-dev libdb-dev libpcap-dev xz-utils libexpat-dev wget

RUN wget http://python.org/ftp/python/2.7.13/Python-2.7.13.tar.xz && tra xvf Python-2.7.13.tar.xz && cd Python-2.7.13 && ./configure --enable-shared --enable-unicode=ucs4 && make && make install

COPY libiconv-1.14 skyeye-1.3.5_rc1 /opt
