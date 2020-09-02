FROM alpine:latest
LABEL maintaner="shadow1163 - https://github.com/shadow1163/"

RUN sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories && \
	apk update && \
	apk upgrade && \
	apk add --no-cache postfix cyrus-sasl cyrus-sasl-plain cyrus-sasl-login rsyslog && \
	(rm /tmp/* 2>/dev/null || true) && (rm -rf /var/cache/apk/* 2>/dev/null || true)

EXPOSE 25

COPY smtpd.conf /etc/sasl2
COPY main.cf /etc/postfix
COPY rsyslog.conf /etc/rsyslog.conf
COPY entrypoint.sh /root

ENTRYPOINT ["/root/entrypoint.sh"]
