#!/usr/bin/env python2.7  
# script by Alex Eames http://RasPi.tv/  
# http://raspi.tv/2013/how-to-use-interrupts-with-python-on-the-raspberry-pi-and-rpi-gpio  
import RPi.GPIO as GPIO
import subprocess
import time

running = True
GPIO.setmode(GPIO.BOARD)  
  
# GPIO 23 set up as input. It is pulled up to stop false signals  
GPIO.setup(18, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)  #pin 5 wPi
GPIO.setup(16, GPIO.OUT, initial= GPIO.LOW)  #pin 4 wPi
   
while running:
    GPIO.wait_for_edge(18, GPIO.RISING)
    time.sleep(0.5)
    if GPIO.input(18):
        running = False
        
command = "/usr/bin/sudo /sbin/shutdown now"
process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
output = process.communicate()[0]
