#include <Adafruit_BME280.h>
#include <ArduinoJson.h>
#include <JeeLib.h>
#include <PortsLCD.h>
#include <SimpleTimer.h>

#define DEBUG 0
#if DEBUG
#define serialPeriod      10000
#define screenPeriod       2000
#else
#define serialPeriod     600000
#define screenPeriod      10000
#endif
Adafruit_BME280 bme; // I2C
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);
StaticJsonBuffer<65> jsonBuffer;
JsonObject& payload = jsonBuffer.createObject();
SimpleTimer timer;

void readOverTheAir(){
  // Check if received msg
  if (rf12_recvDone() && rf12_crc == 0) {
        for (byte i = 0; i < rf12_len; ++i) {
          Serial.print((char) rf12_data[i]);
        }
        Serial.println();
  }
}

void readSensor(){
  // Get sensor data
  payload["t"] = bme.readTemperature();
  payload["h"] = bme.readHumidity();
  payload["p"] = bme.readPressure() ;
}

void printToSerial(){
  payload.printTo(Serial);
  Serial.println();
  }

void printToLCD(){
  lcd.setCursor(11, 0);
  lcd.print((int)payload["t"]);
  lcd.setCursor(11, 1);
  lcd.print((int)payload["h"]); 
}

void setup() {
  // Sensor init
  bme.begin();

  // Serial init
  Serial.begin(9600);

  // Wireless interface init
  rf12_initialize(1, RF12_868MHZ, 201);

  // LCD Screen init
  lcd.begin(16, 2);
  lcd.print("Temp:         *C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity:     % ");

  // Setting up data
  payload["id"] = "liv";
  
  // Setup timer callbacks
  timer.setInterval(screenPeriod, readSensor);
  timer.setInterval(screenPeriod, printToLCD);
  timer.setInterval(serialPeriod, printToSerial);
  readSensor();
  printToLCD();
  printToSerial();
}

void loop() {
 readOverTheAir();
 timer.run();
}

