import serial
import time
import sys

i = 0
while True:
    ser = serial.Serial('/dev/ttyACM0',115200)
    i +=1
    ser.write(str.encode("s{:4}".format(i)))
    print("s{:4}".format(i))
    time.sleep(0.01)
    
#running = True;
#while running:  # Or: while ser.inWaiting():
#    if ser.in_waiting !=0:
#        read_serial= ser.readline()
#        print (read_serial)