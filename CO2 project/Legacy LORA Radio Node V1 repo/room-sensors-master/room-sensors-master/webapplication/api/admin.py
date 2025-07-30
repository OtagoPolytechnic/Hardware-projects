from django.contrib import admin
from .models import Sensor, History

# Register your models here.

admin.site.register(Sensor)
admin.site.register(History)