# https://tutorial.djangogirls.org/en/django_forms/
from django import forms
from .models import Sensor_data

class Sensor_form(forms.ModelForm):

    class Meta:
        model = Sensor_data
        fields = ('temp_soil', 'temp_room', 'humidity', 'moisture', 'lux') #timestamp set in code in views.py