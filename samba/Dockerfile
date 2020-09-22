FROM alpine:latest
LABEL maintaner="shadow1163 - https://github.com/shadow1163/"

RUN sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories && \
	apk update && \
	apk upgrade && \
	apk add --no-cache samba samba-common-tools supervisor&& \
	(rm /tmp/* 2>/dev/null || true) && (rm -rf /var/cache/apk/* 2>/dev/null || true)

# create a dir for the config and the share
RUN mkdir /config /shared

# copy config files from project folder to get a default config going for samba and supervisord
COPY *.conf /config/

RUN addgroup smbgroup && adduser -D -H -G smbgroup -s /bin/false -u 1000 junbo

RUN echo -e "password\npassword" | smbpasswd -a -s -c /config/smb.conf junbo

# exposes samba's default ports (135 for End Point Mapper [DCE/RPC Locator Service],
# 137, 138 for nmbd and 139, 445 for smbd)
EXPOSE 135/tcp 137/udp 138/udp 139/tcp 445/tcp

ENTRYPOINT ["supervisord", "-c", "/config/supervisord.conf"]
