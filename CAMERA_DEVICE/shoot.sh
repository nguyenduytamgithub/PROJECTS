#!/bin/sh

echo "Killall mjpg_streamer"
killall mjpg_streamer

echo "Start fucking mjpg_streamer"
/usr/local/bin/mjpg_streamer -i "input_uvc.so -r 1280x720 -d /dev/video0 -f 30 -q 100" -o "output_http.so -p 8080 -w /usr/local/share/mjpg-streamer/www" > /dev/null 2>&1 &

echo "Start setup camera"

echo "Init..."
sleep 10
echo "Init...OK"
while true; do 
	datetime="$(date +%Y-%m-%d-%H_%M_%S)"
	filename="/home/pi/NDT/dataset/class-${datetime}.jpg"
	echo "We will export to ${filename}"
	echo -e "class-${datetime}" >> /home/pi/NDT/dataset/dataset.csv  > /home/pi/NDT/synch/image/image_name.txt
	wget http://127.0.0.1:8080/?action=snapshot -O ${filename}
	cp ${filename}  /home/pi/NDT/synch/image/image.jpg
	sleep 1
	echo "Start synch image"
	python /home/pi/NDT/synch/code/publish2.py
	echo "Finish synch image - go to sleep"
	sleep 10
done

exit 0

