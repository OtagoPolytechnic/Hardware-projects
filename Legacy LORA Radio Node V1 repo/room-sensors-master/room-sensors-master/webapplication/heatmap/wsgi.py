"""
WSGI config for heatmap project.

It exposes the WSGI callable as a module-level variable named ``application``.

For more information on this file, see
https://docs.djangoproject.com/en/2.0/howto/deployment/wsgi/
"""

import os

from django.core.wsgi import get_wsgi_application
import api
import logging
logging.error("aaaa")

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "heatmap.settings")

application = get_wsgi_application()
