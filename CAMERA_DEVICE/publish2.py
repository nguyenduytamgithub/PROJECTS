import os
import sys
import paho.mqtt.client as mqtt
import json
import base64
THINGSBOARD_HOST = '115.79.27.129'
#THINGSBOARD_HOST = '192.168.1.19'
client = mqtt.Client()
# Connect to ThingsBoard using default MQTT port and 60 seconds keepalive interval
client.connect(THINGSBOARD_HOST, 1883, 60)

client.loop_start()

try:
   
        # Sending humidity and temperature data to ThingsBoard
        #client.publish('synch', json.dumps('ok'), 1)
	f1=open("../image/image.jpg", "rb") #3.7kiB in same folder
	fileContent = f1.read()
	#imageArray = bytearray(fileContent)
	imageArray = base64.b64encode(fileContent).replace('\n','')
	#client.publish('synch', imageArray, 1)
        f2=open("../image/image_name.txt", "rb") #3.7kiB in same folder
        imageName = f2.read().replace('\n','')
	send_msg = {
        	'image_name': imageName,
		'image_array': imageArray
	}
	client.publish('synch',json.dumps(send_msg), 0)	
except KeyboardInterrupt:
    pass

client.loop_stop()
client.disconnect()

