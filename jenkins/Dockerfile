FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN apt-get update && apt-get install -y --fix-missing python3 software-properties-common wget python3-pip openjdk-8-jre openjdk-8-jre-headless apt-utils

#RUN apt-add-repository -y ppa:qameta/allure && apt-get update && apt-get install -y allure apt-transport-https
RUN wget https://dl.bintray.com/qameta/generic/io/qameta/allure/allure/2.6.0/allure-2.6.0.tgz -O /tmp/allure.tgz -o /dev/null && tar xf /tmp/allure.tgz -C /tmp/ && cp -r /tmp/allure-2.6.0/* /usr/local/ && rm -rf /tmp/allure*

RUN wget -q -O - https://pkg.jenkins.io/debian/jenkins-ci.org.key | apt-key add - && echo deb https://pkg.jenkins.io/debian-stable binary/ | tee /etc/apt/sources.list.d/jenkins.list && apt-get update && apt-get install -y jenkins

RUN pip3 install allure-pytest

RUN mkdir -p /var/run/jenkins/ && touch /var/run/jenkins/jenkins.pid

CMD service jenkins restart; tail -f /var/log/jenkins/jenkins.log
