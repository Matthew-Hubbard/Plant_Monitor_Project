from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt #fix this to make more secure... 
from .forms import Sensor_form
from django.utils import timezone 

# Create your views here.

def index(request):
    return HttpResponse("Hello, world. You're at the sensors index.")

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

    return HttpResponse("Hello, world. You're at send data.")