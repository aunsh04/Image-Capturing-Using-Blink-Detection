import time
import RPi.GPIO as GPIO
import os
import subprocess
import bluetooth
import lightblue
from PIL import Image
from subprocess import call

GPIO.setmode(GPIO.BOARD)

GPIO.setup(8, GPIO.IN)
count=0
img=0

# we should know
target_name = "Raspi"
file_to_send = "/home/pi/Desktop/Image/image1.jpg"
# we don't know yet
obex_port = None
target_address = None

print "searching for nearby devices..."
nearby_devices = bluetooth.discover_devices()

for bdaddr in nearby_devices:
   print bluetooth.lookup_name( bdaddr )
   if target_name == bluetooth.lookup_name( bdaddr ):
call(["espeak","Device named raspi has been found!"])
      print "found the target device!"
      target_address = bdaddr
      break

print "searching for the object push service..."
services = lightblue.findservices(target_address)
for service in services:
   if service[2] == "OBEX Object Push":
      obex_port = service[1]
print "OK, service '", service[2], "' is in port", service[1], "!"
      break

client=lightblue.obex.OBEXClient(target_address,12)
client.connect()

#Infrared Detection Code ---------------------------------------
while True:
    time.sleep(0.4)
if(GPIO.input(8)==False):
        print "Open"
        count=0

    else:
        count=count+1
        if(count>=3):
            img=img+1
            print "Close"
            os.chdir("/home/pi/Desktop/Image")
  call(["espeak","Capturing Image!"])
            call(["fswebcam","--fps","15","-r","640x480","-S 8","image1.jpg"])

            print "sending a file..."

            client.put({"name":"/home/pi/Desktop/Image/image1.jpg", "type":"ima$
            call(["espeak","Image sent to the device"])

            time.sleep(1)
            count=0
GPIO.cleanup()
