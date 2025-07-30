FROM python:3.6

RUN mkdir -p /usr/src/app

WORKDIR /usr/src/app

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

# copy current dir's content to container's WORKDIR root i.e. all the contents of the web app
COPY . .

CMD python manage.py migrate; python manage.py collectstatic --no-input; gunicorn heatmap.wsgi -b 0.0.0.0:8000 --log-level debug --capture-output --log-file - --error-logfile - --enable-stdio-inheritance 

EXPOSE 8000
