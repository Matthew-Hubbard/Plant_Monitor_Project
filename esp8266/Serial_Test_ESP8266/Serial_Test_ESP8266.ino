
const int SERIAL_DELAY = 2000;
const int SERIAL_ITTER = 500;

String data = "";

int recieve_data(String & data);

void setup()
{
 Serial.begin(9600);
}

void loop(){
  data = "";

  // Get data from Arduino Sensor
  recieve_data(data);

/*
  boolean StringReady = false;
  Serial.write("Hello from ESP");
  delay(2000);
  while (Serial.available()){
    IncomingString=Serial.readString();
    StringReady= true;
  }
 
  if (StringReady){
    Serial.println("Received String: " + IncomingString);
  }
  */
  delay(500);
}

 int recieve_data(String & data)
 {
   data = "";
   int i = 0;

   // Wait for Serial read buffer to be empty...
   while(!Serial.available() && i < SERIAL_ITTER) {++i;} 

   if(i == SERIAL_ITTER - 1 )
   {
     // Serial not available to read! (read buffer not empty!)");
     return -1;
   }

   //get data from Arduino Sensor
   i = 0;
   while (Serial.available() && i < SERIAL_ITTER)
   //while (Serial.available())
   {
     data = Serial.readString();
     ++i;
   }

   if(i >= SERIAL_ITTER - 1)
   {
     //[ERROR] : Couldn't recieve data
     return -2;
   }

  // Send data back to Arduino Sensor to confirm transmition

  int num_bytes = Serial.availableForWrite(); // check if we have anything still in write buffer
  if(num_bytes > 0)
  {
    // [ERROR] : bytes already in write buffer...
    Serial.flush();
  }
  
  // Sending data over serial to arduino sensor
  delay(SERIAL_DELAY);
  if(data != "")
    Serial.write(data.c_str());
  return 0;
 }
