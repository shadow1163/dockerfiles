FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN apt-get update && apt-get install -y build-essential libncurses5-dev python unzip git wget gawk zlib1g-dev

RUN git clone https://git.openwrt.org/openwrt/openwrt.git/ && cd openwrt && ./scripts/feeds update -a && ./scripts/feeds install -a
