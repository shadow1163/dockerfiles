FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y gcc-arm-none-eabi curl build-essential flex bison bc

#get u-boot source code

RUN curl ftp://ftp.denx.de/pub/u-boot/u-boot-2018.11.tar.bz2 -o u-boot-2018.11.tar.bz2
