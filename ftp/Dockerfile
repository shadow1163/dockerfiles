FROM ubuntu:20.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list &&\
	sed -i s@/security.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list &&\
	apt-get clean &&\
	apt-get update && apt-get install -y vsftpd

RUN mkdir -p /ftp/Downloads && mkdir -p /ftp/Upload && chown -R nobody:nogroup /ftp && chmod 755 /ftp/Downloads && chmod 777 /ftp/Upload && mkdir -p /var/run/vsftpd/empty

ADD vsftpd.conf /etc/vsftpd.conf

CMD ["/usr/sbin/vsftpd", "/etc/vsftpd.conf"]
