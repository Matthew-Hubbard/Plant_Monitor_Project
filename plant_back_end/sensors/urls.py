from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('send_data/', views.send_data, name='send_data'),
]