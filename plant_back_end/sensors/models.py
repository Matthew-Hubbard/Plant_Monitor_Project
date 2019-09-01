from django.db import models
from datetime import datetime

# Create your models here.

class Sensor_data(models.Model):

    def __str__(self):
        return ("\
            \nid:\t\t{0:d}\
            \nsensor_id:\t{1:f}\
            \ntemp_soil:\t{2:f}\
            \ntemp_room:\t{3:f}\
            \nhumidity:\t{4:f}\
            \nheat_index:\t{5:f}\
            \nmoisture:\t{6:f}\
            \nlux:\t\t{7:f}\
            \nvisible:\t{8:f}\
            \nir:\t{9:f}\
            \nfull:\t{10:f}\
            \ntimestamp:\t{11:s}\n"\
            .format(self.id, self.sensor_id, self.temp_soil, self.temp_room, self.humidity, \
                self.heat_index, self.moisture, self.lux, self.visible, self.ir, self.full, \
                self.timestamp.strftime("%m-%d-%y %I:%M:%S%p")))

    sensor_id = models.IntegerField() # Sensor ID to gather data from different sensors
    temp_soil = models.FloatField() # Soil reading from waterproof temp sensor wired probe (fahrenheit)
    temp_room = models.FloatField() # Temp reading from DHT-22 sensor (fahrenheit)
    humidity = models.FloatField() # humidity reading from DHT-22 sensor (fahrenheit)
    heat_index = models.FloatField() # heat index calc from DHT-22 sensor (fahrenheit)
    moisture = models.FloatField() # moisture reading from capacitance moisture sensor
    lux = models.FloatField() # Lux luminosity reading from light sensor
    visible = models.IntegerField() # Visible spectrum light reading from light sensor
    ir = models.IntegerField() # Ifrared spectrum light reading from light sensor
    full = models.IntegerField() # Full spectrum light reading from light sensor
    timestamp = models.DateTimeField('The time/date this data was recorded') # from django.utils import timezone ... timezone.now()
