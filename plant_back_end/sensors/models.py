from django.db import models
from datetime import datetime

# Create your models here.

class Sensor_data(models.Model):

#    class Meta:
#        verbose_name = _("")
#
#        return self.name
#
#    def get_absolute_url(self):
#        return reverse("_detail", kwargs={"pk": self.pk})
    
    def __str__(self):
        return ("\
            \nid:\t\t{0:d}\
            \ntemp_soil:\t{1:f}\
            \ntemp_room:\t{2:f}\
            \nhumidity:\t{3:f}\
            \nmoisture:\t{4:f}\
            \nlux:\t\t{5:f}\
            \ntimestamp:\t{6:s}\n"\
            .format(self.id, self.temp_soil, self.temp_room, self.humidity, self.moisture, self.lux, \
                self.timestamp.strftime("%m-%d-%y %I:%M:%S%p")))

    temp_soil = models.FloatField()
    temp_room = models.FloatField()
    humidity = models.FloatField()
    moisture = models.FloatField()
    lux = models.FloatField()
    timestamp = models.DateTimeField('The time/date this data was recorded') # from django.utils import timezone ... timezone.now()