import serial
import json
import requests

apikey = "R8RLCDLF933DU7K4" 
ser = serial.Serial('/dev/ttyACM0', 9600)
#count = 100
#while count > 0 :
while True: 
   try:
        line = ser.readline()
        data = json.loads(line)
        request = "https://api.thingspeak.com/update?api_key=" + apikey + "&field1=" + str(data["temp"]) + "&field2=" + str(data["humidity"]) + "&field3=" + str(data["pressure"])
        res = requests.get(request)
   except:
        pass
