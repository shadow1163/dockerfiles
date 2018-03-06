FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN apt-get update && apt-get install -y curl build-essential erlang

RUN curl http://tsung.erlang-projects.org/dist/tsung-1.7.0.tar.gz -o /opt/tsung.tar.gz && cd /opt && tar zxf tsung.tar.gz && cd tsung-1.7.0 && ./configure && make && make install
