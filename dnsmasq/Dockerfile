FROM alpine
MAINTAINER shadow1163 <674602286@qq.com>

RUN apk --no-cache add dnsmasq

COPY dnsmasq.conf /etc/
COPY resolv.dnsmasq.conf /etc/

#VOLUME /etc/dnsmasq.hosts

EXPOSE 53/udp

ENTRYPOINT ["dnsmasq", "-k"]
