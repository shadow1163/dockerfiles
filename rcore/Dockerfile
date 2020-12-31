FROM alpine:latest
LABEL maintaner="shadow1163 - https://github.com/shadow1163/"

RUN sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories && \
	apk update && \
	apk upgrade && \
	apk add --update --no-cache wget gcc clang-dev musl-dev yaml curl libmicrohttpd libuuid

RUN mkdir deps && \
    wget -O - https://static.rust-lang.org/dist/rust-nightly-$(apk --print-arch)-unknown-linux-musl.tar.gz | tar -C deps -z -x -f - && \
    sh /deps/rust-nightly-$(apk --print-arch)-unknown-linux-musl/install.sh --prefix=/usr && \
    rm -rf /deps
