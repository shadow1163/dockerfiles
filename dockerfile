# Pull base image
FROM ubuntu:16.04

MAINTAINER shadow1163 "674602286@qq.com"

RUN echo "deb http://cn.archive.ubuntu.com/ubuntu/ xenial main restricted" > /etc/apt/sources.list

RUN "apt-get update && apt-get install php phpmyadmin mysql-server mysql"
