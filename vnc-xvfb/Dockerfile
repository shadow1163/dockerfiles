FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

ENV TZ 'Asia/Shanghai'
ENV DISPLAY :20
ENV SCREEN 0
ENV DISPLAY_MODE 1024x768x16

#Install tools
RUN apt-get update && echo $TZ > /etc/timezone && apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y tzdata && rm /etc/localtime && ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && \
    dpkg-reconfigure -f noninteractive tzdata && apt-get -y dist-upgrade && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --fix-missing python3 python3-pip git xvfb x11vnc xfce4 xfce4-goodies

#Get novnc code
RUN git clone https://github.com/novnc/noVNC.git /root/noVNC && git clone https://github.com/kanaka/websockify /root/noVNC/utils/websockify

RUN ln -s /usr/bin/python3 /usr/bin/python && ln -s /usr/bin/pip3 /usr/bin/pip 

RUN pip3 install numpy

EXPOSE 5920 6080

COPY entrypoint.sh /
RUN chmod ug+x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
