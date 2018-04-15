FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

#COPY sources.list /etc/apt/sources.list

RUN apt-get update && apt-get install -y software-properties-common 

RUN add-apt-repository -y ppa:gophers/archive

RUN mkdir /go

ENV GOPATH=/go

RUN apt-get update && apt-get install -y golang-1.10-go golang-1.10-doc git vim portaudio19-dev mesa-common-dev xorg-dev libgl-dev openssh-server --fix-missing

RUN ln -s /usr/lib/go-1.10/bin/go /usr/bin/go

RUN go get github.com/fogleman/nes

#ssh
RUN mkdir /var/run/sshd
RUN echo "root:password" | chpasswd
RUN sed -i 's/prohibit-password/yes/' /etc/ssh/sshd_config
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

#set go env
RUN echo "export GOPATH=/go" >> /etc/profile
RUN echo "export GOBIN=/go/bin" >> /etc/profile
RUN echo "export PATH=$PATH:$GOBIN" >> /etc/profile

EXPOSE 22
CMD ["/usr/sbin/sshd","-D"]
