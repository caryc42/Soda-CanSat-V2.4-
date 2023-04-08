//Code for Transmitter & Reciever (Talker)
//Wiring
//FS100A ==> Arduino
//GND --> GND
//Data --> D12
//VCC --> 5V

//XLC-RF-5V ==> Arduino
//GND --> GND
//Data --> D11
//VCC --> 5V

//i2C module (on lcd screen) ==> Arduino
//GND --> GND
//VCC --> 5V
//SDA --> SCL
//SCL --> SDA

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>
//include lcd i2c library
#include <LiquidCrystal_I2C.h>

//Set the LCD address:
//For PCF8574T Serial Chip uncoment below (16 chars and 2 line display)
LiquidCrystal_I2C lcd(0x27,16,2);
//For PCF8574AT Serial Chip uncoment below (16 char and 2 line dislpay)
//LiquidCrystal_I2C lcd(0x3F,16,2);

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

int var = 0;
int mode = 0;
int count = 0;
int initloop = 0;
int startup = 1000;
int dit = 5;
int dah = 10;
int selector = 0;
int replywait = 500;

void setup()
{
  // Initialize ASK Object
  rf_driver.init();

  //intialize LCD
  lcd.init();
  lcd.init();

  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("CanSat");
  lcd.setCursor(0,1);
  lcd.print("Base Station");
  delay(startup);
  
  // Setup Serial Monitor
  Serial.begin(9600);
  Serial.println("Intializing Radio...");
  lcd.clear();
  lcd.print("Initializing");
  lcd.setCursor(0,1);
  lcd.print("Radio ...");
  delay(startup);
  
  lcd.clear();
  Serial.println("Radio Intialized!");
  lcd.print("      Radio");
  lcd.setCursor(0,1);
  lcd.print("   Intialized!");
  delay(startup);
  lcd.clear();
}

void loop()
{
  //****MODE 0 - Query and response****
  if (mode == 0){
    if (initloop == 0){
      Serial.println("Mode 0: Query");
      lcd.print("Mode 0: Query");
      delay(2000);
      initloop = 1;
    }

    if (selector == 0) {
      //Transmit query 1
      //const char *msg = "Hello World";
      const char *msg = "Hello A100?";

      //morse train (send 3 times)
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      delay(dit);
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      delay(dit);
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      delay(dit);    
      
      selector = 1;

      Serial.print("Target Address: ");
      Serial.println("A100");

    }
    else if (selector == 1) {
      //Transmit query 2
      //const char *msg = "Hello World";
      const char *msg = "Hello A200?";

      //morse train (send 3 times)
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      delay(dit);
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      delay(dit);
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      delay(dit);

      Serial.print("Target Address: ");
      Serial.println("A200");
      
      selector =0;
     
    }

    //tell lcd screen that query train has been sent
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Sending Query...");
    Serial.print("Sending Query ");
    Serial.print(count);
    Serial.println(" ...");
    count = count + 1;
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print("Query #:");
    lcd.setCursor(9,1);
    lcd.print(count);
    delay(2000);


    //Listen for reply
    var = 0;
    while (var < replywait){
  
    //****Receive****
    // Set buffer to size of expected message (27 characters max)
    uint8_t buf[11];
    uint8_t buflen = sizeof(buf);
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen))
    {

      // Message received with valid checksum
      lcd.setCursor(0,0);
      lcd.print("Info Received:  ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print((char*)buf);
      Serial.print("Message Received: ");
      Serial.println((char*)buf);
      count = 0;
      delay(2000);
    

    }
    delay(5);
    //Just for debugging, to see how long it takes for response
    //Serial.println(var);
    var++;
  
    }
  }

  else if (mode == 1){
    if (initloop == 0){
      Serial.println("Mode 1 - listener");
      lcd.print("Query Mode 1");
      lcd.setCursor(0,1);
      lcd.print("Listening...");
      delay(4000);
      initloop = 1;
    }

    //****Receive****
    // Set buffer to size of expected message (27 characters max)
    uint8_t buf[11];
    uint8_t buflen = sizeof(buf);
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen))
    {

      // Message received with valid checksum
      lcd.setCursor(0,0);
      lcd.print("Message Received:");
      lcd.setCursor(0,1);
      lcd.print(buflen);
      Serial.print("Message Received: ");
      Serial.println((char*)buf);
      count = 0;
      delay(2000);
    }

    
  }
}
