#include <RF24Network.h>
#include "RF24.h"
#include <SPI.h>

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8
RF24 radio(7, 8);
RF24Network network(radio);

const uint16_t master_node = 00;
const uint16_t this_node = 01;

struct packet
{
  uint8_t sensor_id;
  uint8_t packet_number;
  char data[30];
};

uint8_t sensor_id = 1;

String payload1 = "";
String payload2 = "";

void print_packet(packet &my_packet);

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  //radio.setPALevel(RF24_PA_MIN);

  // Open a writing and reading pipe on each radio, with opposite addresses

  //  radio.openWritingPipe(addresses[1]);
  //  radio.openReadingPipe(1, addresses[0]);

  //radio.openWritingPipe(master_address);
  //radio.openReadingPipe(sensor_id, my_address);

  // Start the radio listening for data
  //radio.startListening();

  Serial.println("[Arduino] : nrf2401+ setup complete.");
}

void print_packet(packet &my_packet)
{
  Serial.print(my_packet.sensor_id);
  Serial.print(", ");
  Serial.print(my_packet.packet_number);
  Serial.print(", ");
  Serial.println(my_packet.data);
}

void loop()
{
  network.update();

  packet request_packet;
  strcpy(request_packet.data, "");

#ifdef DEBUG_RF
  Serial.println("[Arduino] : Waiting for request from master");
#endif
  while (network.available())
  {
    RF24NetworkHeader header_in;
    network.read(header_in, &request_packet, 32);
    String payload(request_packet.data);
    Serial.print("[Master] : ");
    print_packet(request_packet);
  }

  delay(100);

  network.update();
  if (strcmp(request_packet.data, "request") == 0)
  {
#ifdef DEBUG_RF
    Serial.println("[Arduino] : Got data request from master.");
#endif
    //if we've been requested to give data...

    packet data_out;
    data_out.sensor_id = sensor_id;

    RF24NetworkHeader header_out(master_node); // (Adress where the data is sent)

    if (request_packet.packet_number == 1)
    {
      //gather data
      payload1 = "1;11.11;22.22;33.33;";
      payload2 = "44.44;55.55;66.66;77.77;";

      data_out.packet_number = 1;
      payload1.toCharArray(data_out.data, 30);

      Serial.print("[Arduino] : Sending ");
      print_packet(data_out);

      bool ok = network.write(header_out, &data_out, 32); // Sending the data to master_node
    }
    else if (request_packet.packet_number == 2 && payload2 != "")
    {
      data_out.packet_number = 2;
      payload2.toCharArray(data_out.data, 30);

      Serial.print("[Arduino] : Sending ");
      print_packet(data_out);

      bool ok = network.write(header_out, &data_out, 32); // Sending the data to master_node
    }
  }

  if(request_packet.packet_number == 2)
  {
    Serial.print("[Arduino] : Sleeping");
    for(int i = 0; i < 10; ++i)
    {
      Serial.print(".");
      delay(100);
    }
    Serial.println();
  }
} // Loop