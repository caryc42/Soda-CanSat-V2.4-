//Code for Transmitter & Reciever (listener)
//Wiring
//FS100A ==> Arduino
//GND --> GND
//Data --> D12
//VCC --> 5V

//XLC-RF-5V ==> Arduino
//GND --> GND
//Data --> D11
//VCC --> 5V

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

//*******setup DHT sensor*****
#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE 22
DHT dht(DHTPIN,DHTTYPE);

//mode 0: Query
//mode 1: const tranmsision
const int mode = 0;
int counter = 0;
//Morese counting
int dit = 5;
int dah = 100;
//cansat address
String address = "A200";

void setup()
{
  // Initialize ASK Object
  rf_driver.init();
  // Setup Serial Monitor
  Serial.begin(9600);
  Serial.println("Radio Initialized...");

  //Intitialize DHT Sensor
  dht.begin();
}

void loop()
{
  //DHT sensor
  float h = dht.readHumidity();
  float c = dht.readTemperature();
  float f = dht.readTemperature(true);
  //Serial.println(h);
  //Serial.println(c);
  //Serial.println(f);
  
  //****Receive****
  // Set buffer to size of expected message (27 characters max)
  uint8_t buf[11];
  uint8_t buflen = sizeof(buf);
  //decode message
  String checkstring((char*)buf);
  String checkaddress = checkstring.substring(6,10);
  
  // Check if received packet is correct size
  //if (rf_driver.recv(buf, &buflen))
  // check if recived address is right??
  if ((rf_driver.recv(buf, &buflen)) && (checkaddress == address))
  {

    // Message received with valid checksum
    Serial.print("Message Received: ");
    Serial.println((char*)buf);
    Serial.print("Query Address: ");
    Serial.println(checkaddress);
    delay(1000);

    //Transmit reply
    String string1 = "Temp: ";
    String string2 = "";
    string2.concat(f);
    String string3 = string1 + string2;
    //conversion of string to const char
    const char *msg = string3.c_str();

    //Sudo Morse Encoding (3 dah = cansat1)/ data send
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    delay(dah);
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    delay(dit);
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();

    //Morose indicate temp status
    if (f > 75){
      delay(dit);
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
    }
    else {
      delay(dah);
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
    }
    
    Serial.println("Sending Data:");
    Serial.println(string3);
    //delay(1000);

  }

  if (mode == 1){
    //constant transmission mode
    
    //Transmit reply
    String string1 = "Temp: ";
    String string2 = "";
    string2.concat(f);
    String string3 = string1 + string2;
    //need to test conversion of string to const char (this is what ChatGPT said)?
    const char *msg = string3.c_str();
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    Serial.println("Sending Data:");
    Serial.println(string3);
    //defualt; 1 sec = 1000
    delay(100);
  }

}
