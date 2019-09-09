from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='sensors_index'),
    # path('table/', views.table, name='sensors_table'),
    # path('table/<int:sensor_id>', views.table_sensor, name='sensors_table_id'),
    path('send_data/', views.send_data, name='send_data'),
    path('get_all/<int:sensor_id>', views.sensor_data_all, name='sensor_data_all'),
]