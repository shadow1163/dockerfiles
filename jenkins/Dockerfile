FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN apt-get update && apt-get install -y --fix-missing python3 software-properties-common wget python3-pip openjdk-8-jre openjdk-8-jre-headless

RUN apt-add-repository -y ppa:qameta/allure && apt-get update && apt-get install -y allure apt-transport-https

RUN wget -q -O - https://pkg.jenkins.io/debian/jenkins-ci.org.key | apt-key add - && echo deb https://pkg.jenkins.io/debian-stable binary/ | tee /etc/apt/sources.list.d/jenkins.list && apt-get update && apt-get install -y jenkins

RUN pip3 install allure-pytest
