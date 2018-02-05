
import pygame
import sys
import time
import serial

ser = serial.Serial('/dev/ttyACM1',9600)
pygame.init()
pygame.joystick.init()

print (pygame.joystick.get_count())

_joystick = pygame.joystick.Joystick(0)
_joystick.init()
print (_joystick.get_init())
print (_joystick.get_id())
print (_joystick.get_name())
print (_joystick.get_numaxes())
print (_joystick.get_numballs())
print (_joystick.get_numbuttons())
print (_joystick.get_numhats())
print (_joystick.get_axis(0))

axes = [ 0.0 ] * _joystick.get_numaxes()
buttons = [ False ] * _joystick.get_numbuttons()

keep_alive=True

while keep_alive:
    event = pygame.event.wait()
    if event.type == pygame.QUIT:
        keep_alive = False
    elif event.type == pygame.JOYAXISMOTION:
        e = event.dict
        axes[e['axis']] = e['value']
        if abs(axes[0]) > 0.2:
            message = "r&{}!".format(int(axes[0]*(axes[3]-1)*50))
            print(message)
            ser.write(str.encode(message))
        elif abs(axes[1]) > 0.2:
            message = "a&{}!".format(int(axes[1]*(axes[3]-1)*50))
            print(message)
            ser.write(str.encode(message))
        else:
            message = "a&0!"
            print(message)
            ser.write(str.encode(message))
    elif event.type in [pygame.JOYBUTTONUP, pygame.JOYBUTTONDOWN ]:
        e = event.dict
        buttons[e['button']] ^= True
