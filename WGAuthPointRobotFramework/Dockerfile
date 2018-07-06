FROM ubuntu:18.04
MAINTAINER shadow1163 (674602286@qq.com)
# Install tools
RUN apt-get update && apt-get -y dist-upgrade && apt-get install -y firefox xvfb python3 python3-pip wget
# Install python3 module
ADD requirements.txt /tmp
RUN pip3 install -r /tmp/requirements.txt
# GeckoDriver v0.19.1
RUN wget -q "https://github.com/mozilla/geckodriver/releases/download/v0.19.1/geckodriver-v0.19.1-linux64.tar.gz" -O /tmp/geckodriver.tgz \
    && tar zxf /tmp/geckodriver.tgz -C /usr/bin/ \
    && rm /tmp/geckodriver.tgz
RUN mkdir /data
ADD test.txt /data
CMD (Xvfb :10 -ac & export DISPLAY=:10; pybot -d /data/reports /data/test.txt)
