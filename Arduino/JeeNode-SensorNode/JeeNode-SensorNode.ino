#include <JeeLib.h>
#include <Adafruit_BME280.h>
#include <ArduinoJson.h>

// set the sync mode to 2 if the fuses are still the Arduino default
// mode 3 (full powerdown) can only be used with 258 CK startup fuses
#define RADIO_SYNC_MODE 2

Adafruit_BME280 bme;
MilliTimer sendTimer;
StaticJsonBuffer<100> jsonBuffer;
JsonObject& payload = jsonBuffer.createObject();
char payloadTest = "Chambre";
double temperature, pressure, humidity;
byte needToSend;
char buffer[256];

void setup() {
  bme.begin();
  payload["sensor"] = "chambre";
  rf12_initialize(2, RF12_868MHZ, 201);
  // rf12_sleep(RF12_SLEEP);
  Serial.begin(9600);
}

void loop() {
  if (rf12_recvDone() && rf12_crc == 0) {
        Serial.print("RX ");
        for (byte i = 0; i < rf12_len; ++i) {
          Serial.print((char) rf12_data[i]);
        }
        Serial.println();
  }
  
  if (sendTimer.poll(1000))
        needToSend = 1;

  if (needToSend && rf12_canSend()) {
      needToSend = 0;
      
      // Read sensors
      // payload["temp"] = bme.readTemperature();
      // payload["pressure"] = bme.readPressure();
      // payload["humidity"] = bme.readHumidity();
      // payload.printTo(buffer, sizeof buffer);
  
      // Send payload over the air
      //rf12_sleep(RF12_WAKEUP);
      rf12_sendStart(0, payloadTest, sizeof payloadTest);
      // rf12_sendStart(1, &buffer, payload.measureLength());
      // rf12_sendWait(RADIO_SYNC_MODE);
      //rf12_sleep(RF12_SLEEP);
  
      Serial.print(buffer);
      Serial.println();
      Serial.flush();
    }
  }
