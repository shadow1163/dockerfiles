FROM ubuntu:18.04

MAINTAINER shadow1163 (674602286@qq.com)

#Install tools
RUN apt-get update && apt-get install -y --fix-missing python git python-virtualenv libffi-dev libssl-dev

#Download code
RUN mkdir /var/www && cd /var/www && virtualenv python && python/bin/pip install django==1.8.6 && python/bin/pip install django-bootstrap3 && git clone gituser@193.46.86.180:/var/repository/jelly.git

#Init DB
RUN cd jelly && mv db.sqlite3 db.sqlite3.org  && ../python/bin/python manage.py migrate

#Create superuser
RUN ../python/bin/python manage.py createsuperuser

EXPOSE 8000

#Enable service
CMD /var/www/python/bin/python manage.py 0.0.0.0:8000
