FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN apt-get update && apt-get install -y curl openjdk-8-jdk openjfx

ARG JMETER_VERSION=4.0

RUN curl http://mirror.bit.edu.cn/apache//jmeter/binaries/apache-jmeter-$JMETER_VERSION.tgz -o /opt/jmeter.tgz && cd /opt && tar zxf jmeter.tgz && rm -f /opt/jmeter.tgz

ENV JMETER_HOME /opt/apache-jmeter-$JMETER_VERSION/

ENV PATH $JMETER_HOME/bin:$PATH

ADD push-demo.jmx $JMETER_HOME/bin

ADD account.csv $JMETER_HOME/bin
