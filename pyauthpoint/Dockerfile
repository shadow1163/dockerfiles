FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

#ENV DISPLAY=20
#ENV SCREEN=0
#ENV DISPLAY_MODE=1024x768x16

#Install tools
RUN apt-get update && echo $TZ > /etc/timezone && apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y tzdata && rm /etc/localtime && ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && \
    dpkg-reconfigure -f noninteractive tzdata && apt-get -y dist-upgrade && \
    apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --fix-missing python3 python3-pip git python-virtualenv firefox xvfb wget openssh-server xfce4 xfce4-goodies x11vnc

# enable root login
RUN mkdir /var/run/sshd
RUN echo 'root:password' | chpasswd
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config && \
    sed -i "s/^.*X11UseLocalhost.*$/X11UseLocalhost no/" /etc/ssh/sshd_config

# SSH login fix. Otherwise user is kicked off after login
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

# install python's module
ADD requirements.txt /
RUN pip3 install -r /requirements.txt

EXPOSE 22 5900

# GeckoDriver v0.19.1
RUN wget -q "https://github.com/mozilla/geckodriver/releases/download/v0.19.1/geckodriver-v0.19.1-linux64.tar.gz" -O /tmp/geckodriver.tgz \
    && tar zxf /tmp/geckodriver.tgz -C /usr/bin/ \
    && rm /tmp/geckodriver.tgz

COPY entrypoint.sh /
RUN chmod ug+x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
