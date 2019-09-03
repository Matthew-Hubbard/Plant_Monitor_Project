#include <SoftwareSerial.h>

SoftwareSerial esp_serial(11, 10); // RX, TX

const int SERIAL_DELAY = 1000;
const int SERIAL_ITTER = 100;

int send_data(const String & data, SoftwareSerial & espSerial);

String data = "";
 
void setup()
{
 Serial.begin(9600);
 esp_serial.begin(9600);
 delay(5000);
 Serial.println("Setup Complete.");
}
 
void loop(){
  /*
  String IncomingString="";
  boolean StringReady = false;
 
  while (esp_serial.available()){
   IncomingString=esp_serial.readString();
   StringReady= true;
  }
 
  if (StringReady){
    Serial.println("[ARDUINO] -> [ESP] : 39");
    esp_serial.write("39");
    Serial.println("[ESP]: \n" + IncomingString);
  }
  */

  int size = 10;
  String data_list [10] = {"10", "20", "30", "40", "50", "60", "70", "80", "90", "100"};
  //data = "10";
  for(int i = 0; i < size; ++i)
  {
    if(send_data(data_list[i], esp_serial) < 0)
    {
      Serial.print("[Arduino] : Couldn't send. Aborting...");
      return;
    }
  }
 
  delay(2000);  
 }

// Send data via software serial
int send_data(const String & data, SoftwareSerial & serial_out)
{
  String recieved = ""; // message back from serial
  int i = 0;
  int num_bytes = serial_out.availableForWrite(); // check if we have anything still in write buffer
  if(num_bytes > 0)
  {
    Serial.print("[Arduino] [ERROR] : Bytes in write buffer. (num_bytes: ");
    Serial.print(num_bytes);
    Serial.println(")");
    Serial.println("Flushing...");
    serial_out.flush();
  }
  
  //Debugging info
  Serial.println("[Arduino] : Writing data over serial to ESP...");
  Serial.println("[Arduino] : " + data + " -> [ESP]");
  
  // Sending data over serial
  serial_out.write(data.c_str());

  delay(SERIAL_DELAY);

  // Now wait for confirmation from ESP. ESP Should send data back to confirm.

  while(!serial_out.available() && i < SERIAL_ITTER) {++i;} // Wait for serial_out read buffer to be empty...

  if(i == SERIAL_ITTER - 1 )
  {
    Serial.print("[Arduino] [ERROR] : serial_out not available to read! (read buffer not empty!)");
    return -1;
  }

  i = 0;
  while(recieved != data && i < SERIAL_ITTER)
  {
    //while (serial_out.available())
    {
      recieved = serial_out.readString();
    }
    ++i;
  }
  if(i >= SERIAL_ITTER - 1)
  {
    Serial.println("[Arduino] [ERROR] : Couldn't recieve confirmation from ESP. (recieved: " + recieved + ")");
    return -2;
  }
  else
  {
    Serial.println("[ESP] : " + recieved);
    if(recieved == data)
      Serial.println("[Arduino] : Transmition success!");
    else
    {
        Serial.println("[Arduino] : Transmition failed.");
    }
    
    serial_out.flush();
    return 0;
  }
}