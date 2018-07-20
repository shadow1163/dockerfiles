FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN apt-get update && apt-get install -y software-properties-common 

RUN add-apt-repository -y ppa:gophers/archive

RUN mkdir /go

ENV GOPATH=/go

RUN apt-get update && apt-get install -y golang-1.10-go golang-1.10-doc git xvfb wget gdebi-core python-minimal python-pip && wget https://dl.google.com/linux/direct/google-chrome-stable_current_amd64.deb -o /dev/null && gdebi -n google-chrome-stable_current_amd64.deb

ENV TZ 'Asia/Shanghai'
RUN apt-get update && echo $TZ > /etc/timezone && apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y tzdata && rm /etc/localtime && ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && dpkg-reconfigure -f noninteractive tzdata && apt-get -y dist-upgrade && DEBIAN_FRONTEND=noninteractive apt-get install -y x11vnc

RUN ln -s /usr/lib/go-1.10/bin/go /usr/bin/go

#set go env
RUN echo "export GOPATH=/go" >> /etc/profile
RUN echo "export GOBIN=/go/bin" >> /etc/profile
RUN echo "export PATH=$PATH:$GOBIN" >> /etc/profile
RUN go get github.com/golang/image; exit 0
RUN mkdir -p $GOPATH/src/golang.org/x/ && ln -s $GOPATH/src/github.com/golang/image/ $GOPATH/src/golang.org/x/image
RUN go get github.com/chromedp/chromedp

#Get novnc code
RUN git clone https://github.com/novnc/noVNC.git /root/noVNC && git clone https://github.com/kanaka/websockify /root/noVNC/utils/websockify

#install python-minimal
RUN pip install numpy

EXPOSE 5901 6080
COPY entrypoint.sh /
RUN chmod ug+x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
