FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN apt-get update && apt-get install -y curl openjdk-8-jdk openjfx

RUN curl http://mirror.bit.edu.cn/apache//jmeter/binaries/apache-jmeter-4.0.tgz -o /opt/jmeter.tgz

RUN cd /opt && tar zxf jmeter.tgz
