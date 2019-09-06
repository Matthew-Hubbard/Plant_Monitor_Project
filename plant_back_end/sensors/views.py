from django.shortcuts import render
from django.http import HttpResponse
from .forms import Sensor_form
from django.utils import timezone 
from django.views.decorators.csrf import csrf_exempt #fix this to make more secure... 
from .models import Sensor_data
from django.http import JsonResponse
from django.core import serializers
from datetime import datetime
from django.db.models import Max

# import json

# Create your views here.

def index(request):
    num_entries = 50
    max_sensor_id = Sensor_data.objects.all().aggregate(Max('sensor_id'))['sensor_id__max']
    print("max_sensor_id: %d" % max_sensor_id)
    # Need to send last 50 entries for each sensor (maybe put into array and iterate from 0 to max_sensor_id)

    data_list = Sensor_data.objects.order_by('-timestamp')[:num_entries]
    soil_temp_data = list(Sensor_data.objects.filter(sensor_id=0).order_by('-timestamp').values_list('temp_soil', flat=True))[:num_entries]
    lux_data = list(Sensor_data.objects.filter(sensor_id=0).order_by('-timestamp').values_list('lux', flat=True))[:num_entries]
    sample_times = list(Sensor_data.objects.filter(sensor_id=0).order_by('-timestamp').values_list('timestamp', flat=True))[:num_entries]
    sample_times_str = []
    for time in sample_times:
        # sample_times_str.append(time.strftime(""))
        sample_times_str.append(time.isoformat())
    print("sample_times_str: \n")
    print(sample_times_str)
    print("\n")
    context = {'data_list': data_list, 'soil_temp_data': soil_temp_data, 'sample_times': sample_times_str, 'lux_data': lux_data}
    return render(request, 'sensors/index.html', context)

def table(request):
#    return HttpResponse("Hello, world. You're at the sensors index.")
    data_list = Sensor_data.objects.order_by('-timestamp')[:50]
    context = {'data_list': data_list}
    return render(request, 'sensors/table.html', context)

def table_sensor(request, sensor_id):
    data_list = Sensor_data.objects.filter(sensor_id=sensor_id).order_by('-timestamp')[:50]
    context = {'data_list': data_list}
    return render(request, 'sensors/table.html', context)

def chart(request):
    return HttpResponse("at sensor/chart")


@csrf_exempt
def send_data(request):
    if request.method == 'POST':
        print ("Got POST request.")
        print("Body: ")
        print(request.body)
        #json_data = json.loads(str(request.body, encoding='utf-8'))
        #print("\njson_data: ")
        #print(json_data['sensor_id'])
        #print("\n")
        #https://tutorial.djangogirls.org/en/django_forms/
        form = Sensor_form(request.POST) #create a form from request.POST (see forms.py)
        if form.is_valid():
            print("form.is_valid() is True.")
            #print("POST: ")
            #print (request.POST)
            print (form)
            post = form.save(commit=False)
            post.timestamp = timezone.now()
            post.save()
            #return HttpResponse("Hello, world. You've made a POST request at /sensors/send_data.")
            return HttpResponse("OK")
        else:
            print("form isn't valid.")
            print(request.body)
            print(form.errors.as_data())
            return HttpResponse(form.errors)

    return HttpResponse("Hello, world. You're at send data.")

def sensor_data_all(request, sensor_id):
    data = serializers.serialize('json',Sensor_data.objects.filter(sensor_id=sensor_id).order_by('-timestamp'))
    return JsonResponse(data, safe=False)