from rest_framework import serializers
from sensors.models import Sensor_data

class Sensor_data_Serialiser(serializers.HyperlinkedModelSerializer):

    class Meta:
        model = Sensor_data
        fields = ('id', 'temp_soil', 'temp_room', 'humidity', 'moisture', 'lux', 'lux', 'timestamp')