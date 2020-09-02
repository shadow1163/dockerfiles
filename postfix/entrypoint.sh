#!/bin/sh -e

## add user
adduser -u 1001 -G postfix -DH -s /sbin/nologin -g "gitea admin user" giteadmin
echo "giteadmin:password" | chpasswd

saslauthd -a shadow
postfix start

rsyslogd -n
