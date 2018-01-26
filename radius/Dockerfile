FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN apt-get update && apt-get install -y wget build-essential libtalloc-dev zlib1g-dev
#libssl-dev

RUN wget https://www.openssl.org/source/openssl-1.0.2n.tar.gz && tar zxf openssl-1.0.2n.tar.gz && cd openssl-1.0.2n && ./config --prefix=/usr --openssldir=/etc/ssl --libdir=lib/openssl-1.0 shared zlib-dynamic && make depend && make && make install

RUN wget ftp://ftp.freeradius.org/pub/freeradius/freeradius-server-3.0.16.tar.gz && tar zxf freeradius-server-3.0.16.tar.gz && cd freeradius-server-3.0.16 && ./configure --with-openssl-lib-dir=/usr/lib/openssl-1.0 --prefix=/ && make && make install

EXPOSE 1812/udp 1813/udp

COPY raddb/clients.conf /etc/raddb/clients.conf

COPY raddb/authorize /etc/raddb/mods-config/files/authorize

CMD ["radiusd", "-X"]
