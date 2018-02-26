FROM ubuntu:16.04

MAINTAINER shadow1163 (674602286@qq.com)

RUN apt-get update && apt-get install -y libpam-radius-auth libpam0g-dev gcc openssh-server

CMD ["sshd", "-D"]
