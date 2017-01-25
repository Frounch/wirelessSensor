/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
***************************************************************************/

#include <Adafruit_BME280.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <JeeLib.h>
#include <PortsLCD.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define DEBUG 0
#if DEBUG
#define serialPeriod       10
#define screenPeriod     2000
#else
#define serialPeriod       60
#define screenPeriod    10000
#endif
Adafruit_BME280 bme; // I2C
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);
StaticJsonBuffer<65> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
double temperature, pressure, humidity;


MilliTimer screenTimer;
int period;

void setup() {
  // Demarre le BME280
  bme.begin();
  Serial.begin(9600);
  rf12_initialize(1, RF12_868MHZ, 201);

  // Demarre l'ecran LCD
  lcd.begin(16, 2);
  lcd.print("Temp:         *C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity:     % ");

  root["id"] = "liv";
  period = 0;
}

void loop() {
  // Check if received msg
  if (rf12_recvDone() && rf12_crc == 0) {
        for (byte i = 0; i < rf12_len; ++i) {
          Serial.print((char) rf12_data[i]);
        }
        Serial.println();
  }
    
  // Maj local sensor output
  if(screenTimer.poll(screenPeriod))
  {
    temperature = bme.readTemperature();
    pressure = bme.readPressure();
    humidity = bme.readHumidity();

    lcd.setCursor(11, 0);
    lcd.print((int)temperature);
    lcd.setCursor(11, 1);
    lcd.print((int)humidity);
    
    period++;
    if(period >= serialPeriod)
    {
      period = 0;
      root["t"] = temperature;
      root["h"] = humidity ;
      root["p"] = pressure;
      root.printTo(Serial);
      Serial.println();
    }
  }

}

