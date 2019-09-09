"""plant_back_end URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path, include
from sensors import views

urlpatterns = [
    path('', views.index, name='home'),
    path('sensors/', include('sensors.urls')),
    path('graphs/', views.graphs, name='graphs'),
    path('table/', views.table, name='table'),
    # path(r'^table/(?P<sensor_id>[0-9]+)$', views.table_sensor, name='table_id'),
    path('table/<int:sensor_id>', views.table_sensor, name='table_id'),
    path('help/', views.help, name='help'),
    path('about/', views.about, name='about'),
    path('admin/', admin.site.urls),
]
