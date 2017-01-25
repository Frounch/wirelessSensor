import serial
import json
import requests

livingApikey = "R8RLCDLF933DU7K4" 
sleepApikey = "SAWRE1F3VGF3B6FW" 
outApikey = "R8RLCDLF933DU7K4" 

ser = serial.Serial('/dev/ttyACM0', 9600)
#count = 100
#while count > 0 :
while True: 
   try:
        line = ser.readline()
        data = json.loads(line)
		if str(data["id"]) == "lvg":
			request = "https://api.thingspeak.com/update?api_key=" + livingApikey + "&field1=" + str(data["temp"]) + "&field2=" + str(data["humidity"]) + "&field3=" + str(data["pressure"])
		elif str(data["id"]) == "slp":
			request = "https://api.thingspeak.com/update?api_key=" + sleepApikey + "&field1=" + str(data["temp"]) + "&field2=" + str(data["humidity"]) + "&field3=" + str(data["pressure"])
		elif str(data["id"]) == "out":
			request = "https://api.thingspeak.com/update?api_key=" + outApikey + "&field1=" + str(data["temp"]) + "&field2=" + str(data["humidity"]) + "&field3=" + str(data["pressure"])
        res = requests.get(request)
   except:
        pass
