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
    context = get_context(1)

    moisture_percent = {}
    dry = 666
    wet = 250
    raw = 0
    for id in context["id_list"]:
        raw = context["moisture_data"][id][0]
        adjusted = (((dry - raw)/(dry - wet)) * 100)
        if(adjusted < 0):
            adjusted = 0
        moisture_percent[id] = adjusted
    context["moisture_percent"] = moisture_percent
    context["home"] = "active"

    return render(request, 'sensors/index.html', context)

def graphs(request):
    context = get_context(50)
    context["graphs"] = "active"
    return render(request, 'sensors/graphs.html', context)

def table(request):
#    return HttpResponse("Hello, world. You're at the sensors index.")
    id_list = list(Sensor_data.objects.values_list('sensor_id', flat=True).distinct())
    data_list = Sensor_data.objects.order_by('-timestamp')[:50]
    context = {'data_list': data_list, 'id_list': id_list, 'title': "All Sensors"}
    context["tables"] = "active"
    return render(request, 'sensors/table.html', context)

def table_sensor(request, sensor_id):
    id_list = list(Sensor_data.objects.values_list('sensor_id', flat=True).distinct())
    data_list = Sensor_data.objects.filter(sensor_id=sensor_id).order_by('-timestamp')[:50]
    context = {'id_list': id_list, 'data_list': data_list, 'title': "Sensor " + str(sensor_id)}
    context["tables"] = "active"
    return render(request, 'sensors/table.html', context)

def chart(request):
    return HttpResponse("at sensor/chart")

def about(request):
    id_list = list(Sensor_data.objects.values_list('sensor_id', flat=True).distinct())
    context = {'id_list': id_list, 'about': "active"}
    return render(request, 'sensors/about.html', context)

def setup(request):
    id_list = list(Sensor_data.objects.values_list('sensor_id', flat=True).distinct())
    context = {'id_list': id_list, 'setup': "active"}
    return render(request, 'sensors/setup.html', context)

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

# helper function to get the context for graphs (last num_entries readings for each arduino sensor)
def get_context(num_entries):
    id_list = list(Sensor_data.objects.values_list('sensor_id', flat=True).distinct())
    sample_times = {}
    sample_times_str = {}
    temp_soil_data = {}
    temp_room_data = {}
    humidity_data = {}
    heat_index_data = {}
    moisture_data = {}
    lux_data = {}
    visible_data = {}
    ir_data = {}
    full_data = {}

    for id in id_list: # Gather last num_entries readings for each arduino 
        # Get sample times for each arudino sensor 
        sample_times[id] = list(Sensor_data.objects.filter(sensor_id=id).order_by('-timestamp').values_list('timestamp', flat=True))[:num_entries]
        sample_times_str[id] = []
        # convert time to ISO
        for time in sample_times[id]:
            sample_times_str[id].append(time.isoformat())

        temp_soil_data[id] = list(Sensor_data.objects.filter(sensor_id=id).order_by('-timestamp').values_list('temp_soil', flat=True))[:num_entries]
        temp_room_data[id] = list(Sensor_data.objects.filter(sensor_id=id).order_by('-timestamp').values_list('temp_room', flat=True))[:num_entries]
        humidity_data[id] = list(Sensor_data.objects.filter(sensor_id=id).order_by('-timestamp').values_list('humidity', flat=True))[:num_entries]
        heat_index_data[id] = list(Sensor_data.objects.filter(sensor_id=id).order_by('-timestamp').values_list('heat_index', flat=True))[:num_entries]
        moisture_data[id] = list(Sensor_data.objects.filter(sensor_id=id).order_by('-timestamp').values_list('moisture', flat=True))[:num_entries]
        lux_data[id] = list(Sensor_data.objects.filter(sensor_id=id).order_by('-timestamp').values_list('lux', flat=True))[:num_entries]
        visible_data[id] = list(Sensor_data.objects.filter(sensor_id=id).order_by('-timestamp').values_list('visible', flat=True))[:num_entries]
        ir_data[id] = list(Sensor_data.objects.filter(sensor_id=id).order_by('-timestamp').values_list('ir', flat=True))[:num_entries]
        full_data[id] = list(Sensor_data.objects.filter(sensor_id=id).order_by('-timestamp').values_list('full', flat=True))[:num_entries]

    context = {'id_list': id_list, 'sample_times': sample_times_str, 'temp_soil_data': temp_soil_data, \
        'temp_room_data': temp_room_data, 'humidity_data': humidity_data, 'heat_index_data': heat_index_data, \
            'moisture_data': moisture_data, 'lux_data': lux_data, 'visible_data':visible_data, 'ir_data': ir_data,\
                'full_data': full_data}
    return context
