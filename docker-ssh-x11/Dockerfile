FROM ubuntu:18.04
RUN apt-get update && apt-get install -y xterm
RUN useradd -ms /bin/bash xterm
USER xterm
WORKDIR /home/xterm
#RUN apt update \
#    && apt install -y firefox \
#                      openssh-server \
#                      xauth \
#    && mkdir /var/run/sshd \
#    && mkdir /root/.ssh \
#    && chmod 700 /root/.ssh
#RUN ssh-keygen -A \
#    && sed -i "s/^.*PasswordAuthentication.*$/PasswordAuthentication no/" /etc/ssh/sshd_config \
#    && sed -i "s/^.*X11Forwarding.*$/X11Forwarding yes/" /etc/ssh/sshd_config \
#    && sed -i "s/^.*X11UseLocalhost.*$/X11UseLocalhost no/" /etc/ssh/sshd_config \
#    && grep "^X11UseLocalhost" /etc/ssh/sshd_config || echo "X11UseLocalhost no" >> /etc/ssh/sshd_config
#
#RUN echo "password" >> /root/.ssh/authorized_keys
#
#ENTRYPOINT ["sh", "-c", "/usr/sbin/sshd && tail -f /dev/null"]
