#! /bin/sh
echo "slapd slapd/no_configuration boolean false" | debconf-set-selections
echo "slapd slapd/domain string example.com" | debconf-set-selections
echo "slapd shared/organization string example" | debconf-set-selections
echo "slapd slapd/internal/generated_adminpw password password" | debconf-set-selections 
echo "slapd slapd/internal/adminpw password password" | debconf-set-selections
echo "slapd slapd/backend select MDB" | debconf-set-selections
echo "slapd slapd/purge_database boolean false" | debconf-set-selections
echo "slapd slapd/move_old_database boolean true" | debconf-set-selections 
echo "slapd slapd/allow_ldap_v2 boolean false" | debconf-set-selections 
echo "slapd slapd/password1 password password" | debconf-set-selections
echo "slapd slapd/password2 password password" | debconf-set-selections
killall slapd
sed -i "s/^exit 101$/exit 0/" /usr/sbin/policy-rc.d
dpkg-reconfigure slapd
killall slapd
service slapd restart
apachectl -DFOREGROUND
