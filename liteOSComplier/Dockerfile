FROM ubuntu:20.04

MAINTAINER shadow1163 (674602286@qq.com)

ENV TZ 'Asia/Shanghai'

RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list &&\
	sed -i s@/security.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list &&\
	apt-get clean && echo $TZ > /etc/timezone &&\
	apt-get update && apt-get install -y gcc-arm-none-eabi python3 python3-pip binutils-arm-none-eabi

RUN ln -s /usr/bin/python3 /usr/bin/python

RUN pip3 install kconfiglib
