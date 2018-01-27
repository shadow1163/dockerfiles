FROM ubuntu:14.04
MAINTAINER 674602286@qq.com

RUN apt-get update \
 && apt-get -y install build-essential automake autoconf libtool pkg-config intltool libcurl4-openssl-dev libglib2.0-dev libevent-dev libminiupnpc-dev libappindicator-dev wget vim unzip

RUN wget http://bigota.miwifi.com/xiaoqiang/sdk/tools/package/sdk_package.zip -O /opt/sdk_package.zip

RUN unzip /opt/sdk_package.zip -d /opt

RUN export PATH="/opt/sdk_package/toolchain/bin/:$PATH"; \
export CC="/opt/sdk_package/toolchain/bin/arm-xiaomi-linux-uclibcgnueabi-gcc"
