FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN apt-get update && apt-get install -y software-properties-common 

RUN add-apt-repository -y ppa:gophers/archive

RUN mkdir /go

ENV GOPATH=/go

RUN apt-get update && apt-get install -y golang-1.10-go golang-1.10-doc git python3 python3-matplotlib python3-pip

RUN ln -s /usr/lib/go-1.10/bin/go /usr/bin/go

#set go env
RUN echo "export GOPATH=/go" >> /etc/profile
RUN echo "export GOBIN=/go/bin" >> /etc/profile
RUN echo "export PATH=$PATH:$GOBIN" >> /etc/profile

