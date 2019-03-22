FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

ENV TERM linux
ENV DEBIAN_FRONTEND noninteractive
ENV LC_ALL C
ENV RUNLEVEL 1
RUN echo 'slapd slapd/password1 password password' | debconf-set-selections && echo 'slapd slapd/password2 password password' | debconf-set-selections && apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get install -y slapd ldap-utils ldap-account-manager

ADD start.sh /start.sh
RUN chmod +x /start.sh
ADD user.ldif /user.ldif

EXPOSE 389 80

CMD /start.sh
