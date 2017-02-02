import serial
import json
import requests

livApikey = "R8RLCDLF933DU7K4" 
outApikey = "SAWRE1F3VGF3B6FW" 
slpApikey = "V8LIS0N796RKN6S0" 

ser = serial.Serial('/dev/ttyACM0', 9600)
#count = 100
#while count > 0 :
print "Starting wireless sensor script"
while True: 
   try:
        line = ser.readline()
        data = json.loads(line)
	if str(data["id"]) == "liv":
		request = "https://api.thingspeak.com/update?api_key=" + livApikey + "&field1=" + str(data["t"]) + "&field2=" + str(data["h"]) + "&field3=" + str(data["p"])
	elif str(data["id"]) == "slp":
		request = "https://api.thingspeak.com/update?api_key=" + slpApikey + "&field1=" + str(data["t"]) + "&field2=" + str(data["h"]) + "&field3=" + str(data["p"])
	elif str(data["id"]) == "out":
		request = "https://api.thingspeak.com/update?api_key=" + outApikey + "&field1=" + str(data["t"]) + "&field2=" + str(data["h"]) + "&field3=" + str(data["p"])
        res = requests.get(request)
   except:
        pass

