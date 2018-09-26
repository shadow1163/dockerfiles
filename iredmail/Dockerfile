FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

ENV IREADVERSION 0.9.8

RUN apt-get update && apt-get install -y wget 

# Get iredmail source code
RUN wget https://bitbucket.org/zhb/iredmail/downloads/iRedMail-${IREADVERSION}.tar.bz2 -O /tmp/iRedMail-${IREADVERSION}.tar.bz2
