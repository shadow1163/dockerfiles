#!/bin/bash

if [ -f /tmp/.X0-lock ]; then
  rm -f /tmp/.X0-lock
fi

#VNC_STORE_PWD_FILE=/root/.vnc/passwd
#if [ ! -e "${VNC_STORE_PWD_FILE}" -o -n "${VNC_PASSWORD}" ]; then
#  mkdir -vp /root/.vnc > /dev/null 2>&1
#  x11vnc -storepasswd ${VNC_PASSWORD:-password} ${VNC_STORE_PWD_FILE} > /dev/null 2>&1
#fi

#x11vnc -xkb -noxrecord -noxfixes -noxdamage -display :$DISPLAY -forever -bg -rfbauth /root/.vnc/passwd -rfbport 5900

/usr/sbin/sshd -D &

#startxfce4 &

exec Xvfb :$DISPLAY -ac -listen tcp -screen $SCREEN $DISPLAY_MODE
