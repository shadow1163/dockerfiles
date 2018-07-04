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
#python3 python3-pip git python-virtualenv firefox xvfb wget openssh-server 
#xfce4 xfce4-goodies x11vnc

EXPOSE 5920

COPY entrypoint.sh /
RUN chmod ug+x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
