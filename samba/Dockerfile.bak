FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

#COPY sources.list /etc/apt/sources.list

RUN apt-get update && apt-get install -y samba samba-common python-glade2 system-config-samba

RUN useradd junbo && addgroup smbgroup && usermod -aG smbgroup junbo && (echo "welcome"; echo "welcome" )| smbpasswd -a junbo

RUN mkdir /Downloads && chown -R root:smbgroup /Downloads && chmod -R 0775 /Downloads

ADD smb.conf /etc/samba/

CMD service smbd restart; tail -f /var/log/samba/log.smbd
