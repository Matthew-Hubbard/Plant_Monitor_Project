from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('send_data/', views.send_data, name='send_data'),
    path('<int:sensor_id>', views.sensor_data_all, name='sensor_data_all'),
]