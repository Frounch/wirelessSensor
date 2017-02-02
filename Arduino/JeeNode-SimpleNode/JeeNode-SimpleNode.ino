#include <Adafruit_BME280.h>
#include <ArduinoJson.h>
#include <JeeLib.h>
#include <PortsLCD.h>
#include <SimpleTimer.h>

#define CHAMBRE 0
#define EXT 1

#if CHAMBRE
#define jeenodeId 3
#define dataId "slp"
#endif
#if EXT
#define jeenodeId 4
#define dataId "out"
#endif

#define jeenodeGroup 201

#define DEBUG 0
#if DEBUG
#define updateInterval    10000
#define SERIAL 1
#else
#define updateInterval   600000
#define SERIAL 0
#endif

byte needToSend;
StaticJsonBuffer<65> jsonBuffer;
JsonObject& payload = jsonBuffer.createObject();
char buffer[65];
Adafruit_BME280 bme;
SimpleTimer timer;

void readSensor(){
  // Read sensors
  payload["t"] = bme.readTemperature();
  payload["h"] = bme.readHumidity();
  payload["p"] = bme.readPressure();
  if(rf12_lowbat())
    payload["lowbat"] = 1;
  else
    payload["lowbat"] = 0;
  
  // Export to buffer
  payload.printTo(buffer, sizeof buffer);
  needToSend = 1;
#if SERIAL
    Serial.println("readSensor");
#endif

}

void sendOTA(){
  rf12_recvDone();
  if (needToSend && rf12_canSend()) {
    rf12_sendStart(0, buffer, payload.measureLength());
    needToSend = 0;
#if SERIAL
    Serial.println("sendOTA");
#endif

  }
}

void printToSerial(){
   // print to serial
  payload.printTo(Serial);
  Serial.println();
}

void setup() {
#if SERIAL
  // Serial init
  Serial.begin(9600);
#endif

  // Wireless interface init
  rf12_initialize(jeenodeId, RF12_868MHZ, jeenodeGroup);
  
  // Payload init
  payload["id"] = dataId;

  // Sensor init
  bme.begin();
  timer.setInterval(updateInterval, readSensor);
  readSensor();

#if SERIAL
  timer.setInterval(updateInterval, printToSerial);
  printToSerial();
#endif
}

void loop() {
   timer.run();
   sendOTA();
}
