#!/bin/sh
rm -r /home/pi/.node-red/flows_raspberrypi.json
mv /home/pi/.node-red/run/flows_raspberrypi2.json /home/pi/.node-red/flows_raspberrypi.json
exit 0