import pyrebase
import RPi.GPIO as GPIO
from time import sleep
import serial
import datetime
from firebase import firebase

import json
import os 

GPIO.setmode(GPIO.BCM)
GPIO.cleanup()
GPIO.setwarnings(False)

config = {
  "apiKey":"AIzaSyDIdkNqQweE_nYE74hExtKQvOepSesNsdA",
  "authDomain":"slbic-system-demo.firebaseapp.com",
  "databaseURL":"https://slbic-system-demo.firebaseio.com",
  "storageBucket":"slbic-system-demo.appspot.com"
}

firebase = pyrebase.initialize_app(config)

ser = serial.Serial(
        port='/dev/ttyS0',
        baudrate = 9600,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1
)

db = firebase.database()

def update_firebase():
    x = ser.readline().strip().decode('utf-8')
    print("Reading:...")
    print (x)
    if len(x) > 12:
       result = x.split(' ')
       node_num = result[0]
       temp_num = result[1]
       motion_num = result[2]
       power_num = result[3]
       led_status = result[4]
       hubID = 'hub1'
       if node_num is not None and temp_num is not None and motion_num is not None and power_num is not None:
          data = {"hubID": hubID, "nodeID":node_num,"temperature": temp_num, "pir": motion_num, "current": power_num, "lampStatus":led_status}
          db.child("Hubs").push(data)
          print ("DONE")

while True:
	update_firebase()
	sleep(2)
