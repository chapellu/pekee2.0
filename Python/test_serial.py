import serial

ser = serial.Serial('/dev/ttyACM0',9600)
ser.write(b"a&100:50:25")
running = True;
while running:  # Or: while ser.inWaiting():
    if ser.in_waiting !=0:
        read_serial= ser.readline()
        print (read_serial)