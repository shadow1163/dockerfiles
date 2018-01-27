FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

ENV TERM linux
ENV DEBIAN_FRONTEND noninteractive
ENV LC_ALL C
ENV RUNLEVEL 1
#RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y slapd ldap-utils phpldapadmin
#RUN apt-get update && apt-get install -y slapd ldap-utils phpldapadmin
RUN echo 'slapd slapd/password1 password password' | debconf-set-selections && echo 'slapd slapd/password2 password password' | debconf-set-selections && apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get install -y slapd ldap-utils phpldapadmin

RUN a2enmod ssl && a2ensite default-ssl.conf

#RUN echo "slapd slapd/no_configuration boolean false" | debconf-set-selections && \
#	echo "slapd slapd/domain string example.com" | debconf-set-selections && \
#	echo "slapd shared/organization string example" | debconf-set-selections && \
#	echo "slapd slapd/internal/generated_adminpw password" | debconf-set-selections && \
#	echo "slapd slapd/internal/adminpw password" | debconf-set-selections && \
#	echo "slapd slapd/backend select MDB" | debconf-set-selections && \
#	echo "slapd slapd/purge_database boolean false" | debconf-set-selections && \
#	echo "slapd slapd/move_old_database boolean true" | debconf-set-selections  && \
#	echo "slapd slapd/allow_ldap_v2 boolean false" | debconf-set-selections && \
#	dpkg-reconfigure slapd
#
### fix invoke-rc.d: could not determine current runlevel
#RUN sed -i "s/^exit 101$/exit 0/" /usr/sbin/policy-rc.d && dpkg-reconfigure slapd

ADD start.sh /start.sh
RUN chmod +x /start.sh

EXPOSE 389 80 443

#CMD service slapd start && apachectl -DFOREGROUND
CMD /start.sh
