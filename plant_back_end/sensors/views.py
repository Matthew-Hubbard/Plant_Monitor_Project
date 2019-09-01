from django.shortcuts import render
from django.http import HttpResponse
from .forms import Sensor_form
from django.utils import timezone 
from django.views.decorators.csrf import csrf_exempt #fix this to make more secure... 
from .models import Sensor_data
from django.http import JsonResponse
from django.core import serializers

# Create your views here.

def index(request):
#    return HttpResponse("Hello, world. You're at the sensors index.")
    data_list = Sensor_data.objects.order_by('timestamp')[:5]
    context = {'data_list': data_list}
    return render(request, 'sensors/index.html', context)

@csrf_exempt
def send_data(request):
    if request.method == 'POST':
        print ("Got POST request.")
        #https://tutorial.djangogirls.org/en/django_forms/
        form = Sensor_form(request.POST) #create a form from request.POST (see forms.py)
        if form.is_valid():
            print("form.is_valid() is True.")
            print (request.POST)
            print (form)
            post = form.save(commit=False)
            post.timestamp = timezone.now()
            post.save()
            #return HttpResponse("Hello, world. You've made a POST request at /sensors/send_data.")
            return HttpResponse("OK")
        else:
            print("form isn't valid.")
            print(request.body)

    return HttpResponse("Hello, world. You're at send data.")

def sensor_data_all(request, sensor_id):
    data = serializers.serialize('json',Sensor_data.objects.order_by('timestamp'))
    return JsonResponse(data, safe=False)
