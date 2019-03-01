#!/usr/bin/python
import os
import sys
import json

def insertChar(mystring, position, chartoinsert ):
	#longi = len(mystring)
	mystring   =  mystring[:position] + chartoinsert + mystring[position:] 
	return mystring  
try:
		f1=open("/home/pi/.node-red/flows_raspberrypi.json", "rb")
		f2=open("/home/pi/.node-red/run/mqtt_broker.json", "rb")
		f3=open("/home/pi/.node-red/run/flows_raspberrypi2.json","w")
		string1 = f1.read().replace('\n','')
		string2 = f2.read().replace('\n','')
		length_str1 = len(string1)
		string_merger = insertChar (string1,length_str1-1,string2)
		f3.write(string_merger)
		print(length_str1)
		print(len(string_merger))
		os.system('sh /home/pi/.node-red/run/reload.sh')
		
except KeyboardInterrupt:
    pass
