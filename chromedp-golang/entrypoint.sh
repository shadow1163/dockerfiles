#!/bin/bash
# entrypoint.sh file for starting the xvfb with better screen resolution, configuring and running the vnc server, pulling the code from git and then running the test.
export DISPLAY=:1
Xvfb :1 -screen 0 1366x768x16 -nolisten tcp &
sleep 5
#x11vnc -storepasswd password /tmp/vncpass 
x11vnc -rfbport 5901 -nopw -ncache 1 -display $DISPLAY -forever &
cd /root/noVNC && ln -s vnc_lite.html index.html && ./utils/launch.sh --vnc localhost:5901
