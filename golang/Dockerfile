FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list &&\
	sed -i s@/security.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list &&\
	apt-get clean &&\
	apt-get update && apt-get install -y software-properties-common
RUN add-apt-repository -y ppa:longsleep/golang-backports &&\
	sed -i s@/ppa.launchpad.net/@/lanuchpad.moruy.cn/@g /etc/apt/sources.list.d/*.list &&\
	apt-get update && apt-get install -y golang-go
