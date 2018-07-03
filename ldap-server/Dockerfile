FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

ENV TERM linux
ENV DEBIAN_FRONTEND noninteractive
ENV LC_ALL C
ENV RUNLEVEL 1
RUN echo 'slapd slapd/password1 password password' | debconf-set-selections && echo 'slapd slapd/password2 password password' | debconf-set-selections && apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get install -y slapd ldap-utils phpldapadmin

RUN apt-get install -y gnutls-bin ssl-cert

RUN mkdir /etc/ssl/templates
ADD ca_server.conf /etc/ssl/templates/ca_server.conf
ADD ldap_server.conf /etc/ssl/templates/ldap_server.conf

RUN certtool -p --outfile /etc/ssl/private/ca_server.key && certtool -s --load-privkey /etc/ssl/private/ca_server.key --template /etc/ssl/templates/ca_server.conf --outfile /etc/ssl/certs/ca_server.pem && certtool -p --sec-param high --outfile /etc/ssl/private/ldap_server.key && certtool -c --load-privkey /etc/ssl/private/ldap_server.key --load-ca-certificate /etc/ssl/certs/ca_server.pem --load-ca-privkey /etc/ssl/private/ca_server.key --template /etc/ssl/templates/ldap_server.conf --outfile /etc/ssl/certs/ldap_server.pem && usermod -aG ssl-cert openldap && chown :ssl-cert /etc/ssl/private/ldap_server.key && chmod 640 /etc/ssl/private/ldap_server.key

ADD addcerts.ldif /addcerts.ldif
RUN service slapd start && ldapmodify -H ldapi:// -Y EXTERNAL -f /addcerts.ldif

RUN a2enmod ssl && a2ensite default-ssl.conf

ADD start.sh /start.sh
RUN chmod +x /start.sh

EXPOSE 389 80 443 636

#CMD service slapd start && 
#CMD apachectl -DFOREGROUND
CMD /start.sh
