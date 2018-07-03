#!/bin/bash
# entrypoint.sh file for starting the xvfb with better screen resolution, configuring and running the vnc server, pulling the code from git and then running the test.
export DISPLAY=:20
Xvfb :20 -screen 0 1024x768x24 -nolisten tcp &
x11vnc -storepasswd password /tmp/vncpass 
x11vnc -rfbport 5920 -rfbauth /tmp/vncpass -display $DISPLAY -forever
#Xvfb :20 -screen 0 1366x768x16 &
#x11vnc -passwd TestVNC -display :20 -N -forever
