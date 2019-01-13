import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(18,GPIO.OUT)

if GPIO.input(18):
	GPIO.output(18,0)
	print "Led OFF";
else:
	GPIO.output(18,1)
	print "Led ON";
