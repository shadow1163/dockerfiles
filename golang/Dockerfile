FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

COPY sources.list /etc/apt/sources.list

RUN mkdir /go

ENV GOPATH=/go

RUN apt-get update && apt-get install -y golang git vim portaudio19-dev mesa-common-dev xorg-dev libgl-dev

RUN go get github.com/fogleman/nes
