#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

/****************** User Config ***************************/

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7, 8);
/**********************************************************/

RF24Network network(radio);

#define NUM_NODES 4
#define MAX_TRIES 100 // max number of tries to request data from sensor before going to the next one

//const uint16_t this_node = 00;
//const uint16_t node01 = 01;
const uint16_t node_array[NUM_NODES] = {00, 01, 02, 03};

struct packet
{
  uint8_t sensor_id;
  uint8_t packet_number;
  char data[30];
};

void setup()
{
  Serial.begin(9600);

  SPI.begin();
  radio.begin();
  //network.begin(90, this_node);
  network.begin(90, node_array[0]);

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  //radio.setPALevel(RF24_PA_MIN);

  // Open a writing and reading pipe on each radio, with opposite addresses
  //radio.openWritingPipe(addresses[0]);
  //radio.openReadingPipe(1, my_address);

  // Start the radio listening for data
  //radio.startListening();
  Serial.println("[Arduino] : nrf2401+ setup complete.");
}

void loop()
{
  //int cur_id = 1;

  for (int cur_id = 1; cur_id < NUM_NODES; ++cur_id)
  {
    bool both_packets = false;
    String payload = "";
    int cur_packet = 1;
    int count = 0;

    while (!both_packets && count < MAX_TRIES)
    {
      ++count;
#ifdef DEBUG_RF
      Serial.print("*");
#endif

      network.update();

      packet request_packet;
      request_packet.sensor_id = cur_id;
      request_packet.packet_number = cur_packet;
      strcpy(request_packet.data, "request");

#ifdef DEBUG_RF
      Serial.print("[Arduino] : Requesting Sensor ");
      Serial.print(cur_id);
      Serial.print(" packet ");
      Serial.println(cur_packet);
#endif

      RF24NetworkHeader header_out(node_array[cur_id]);         // (Adress where the data is sent)
      bool ok = network.write(header_out, &request_packet, 32); // Sending request to sensor

      delay(100);

      network.update();
      while (network.available() && !both_packets)
      {
        //Serial.print("-");
        RF24NetworkHeader header_in;
        packet sensor_data;
        network.read(header_in, &sensor_data, 32);
#ifdef DEBUG_RF
        Serial.print("[Sensor ");
        Serial.print(sensor_data.sensor_id);
        Serial.print("] : ");
        Serial.println(sensor_data.data);
#endif
        if (sensor_data.sensor_id = cur_id && sensor_data.packet_number == cur_packet)
        {
          payload += String(sensor_data.data);
          if (sensor_data.packet_number == 2)
          {
#ifdef DEBUG_RF
            Serial.println("[Arduino] : Got both packets!");
#endif
            both_packets = true;
          }
          ++cur_packet;
        }
        else
        {
#ifdef DEBUG_RF
          Serial.println("[Arduino][ERROR] : Packet didn't match!");
          Serial.print("[Arduino] : both_packets: ");
          Serial.println(both_packets);
#endif
        }
      }
    }
    Serial.print("[Arduino] [Sensor ");
    Serial.print(cur_id);
    Serial.print(" PAYLOAD] : ");
    Serial.println(payload);
  }

  Serial.print("[Arduino] : Sleeping");
  for (int i = 0; i < 10; ++i)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

} // Loop
