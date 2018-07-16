FROM ubuntu:18.04
MAINTAINER shadow1163 (674602286@qq.com)
# Install tools
RUN apt-get update && apt-get -y dist-upgrade && apt-get install -y firefox xvfb python3 python3-pip wget
#ssh
RUN apt-get update && apt-get install -y git openssh-server default-jre
RUN useradd -d /home/jenkins -ms /bin/bash -g root -G sudo jenkins && echo "jenkins:password" | chpasswd
ENV NOTVISIBLE "in users profile"
RUN mkdir /var/run/sshd && sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd && echo "export VISIBLE=now" >> /etc/profile
# Install python3 module
ADD requirements.txt /tmp
RUN pip3 install -r /tmp/requirements.txt
# GeckoDriver v0.19.1
RUN wget -q "https://github.com/mozilla/geckodriver/releases/download/v0.19.1/geckodriver-v0.19.1-linux64.tar.gz" -O /tmp/geckodriver.tgz \
    && tar zxf /tmp/geckodriver.tgz -C /usr/bin/ \
    && rm /tmp/geckodriver.tgz
USER jenkins
WORKDIR /home/jenkins
RUN mkdir /home/jenkins/data
ADD test.txt /home/jenkins/data
USER root
EXPOSE 22
CMD ["/usr/sbin/sshd","-D"]
