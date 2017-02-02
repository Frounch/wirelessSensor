/// @dir rf12serial
/// Serial data over RF12 demo, works in both directions.
// 2009-04-17 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <JeeLib.h>
void setup() {
    Serial.begin(9600);
    Serial.print("\n[rf12serial]");
    
    // this is node 2 in net group 100 on the 868 MHz band
    rf12_initialize(2, RF12_868MHZ, 100);
}

void loop() 
{    
    if (rf12_recvDone() && rf12_crc == 0)
    {
        // good packet received
        Serial.print(" seq ");
        Serial.print(rf12_hdr);
        Serial.print(" =");
        for (byte i = 0; i < rf12_len; ++i) 
        {
            Serial.print(' ');
            Serial.print(rf12_data[i], HEX);
        }
        Serial.println();
   }
}
